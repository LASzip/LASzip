/******************************************************************************
 *
 * Project:  integrating laszip into liblas - http://liblas.org -
 * Purpose:
 * Author:   Martin Isenburg
 *           isenburg at cs.unc.edu
 *
 ******************************************************************************
 * Copyright (c) 2010, Martin Isenburg
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Licence as published
 * by the Free Software Foundation.
 *
 * See the COPYING file for more information.
 *
 ****************************************************************************/

/*
===============================================================================

  FILE:  arithmeticdecoder.cpp
  
  CONTENTS:
      
    see header file

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2009 martin isenburg (isenburg@cs.unc.edu)
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see header file
  
===============================================================================
*/
#include "arithmeticdecoder.hpp"

#include <string.h>
#include <assert.h>

#include "arithmeticmodel.hpp"

ArithmeticDecoder::ArithmeticDecoder()
{
  instream = 0;
}

I32 ArithmeticDecoder::init(ByteStreamIn* instream)
{
  assert(instream);
  this->instream = instream;
  length = AC__MaxLength;
  value = (instream->getByte() << 24);
  value |= (instream->getByte() << 16);
  value |= (instream->getByte() << 8);
  value |= (instream->getByte());
  return 0;
}

void ArithmeticDecoder::done()
{
  instream = 0;
}

EntropyModel* ArithmeticDecoder::createBitModel()
{
  ArithmeticBitModel* m = new ArithmeticBitModel();
  return (EntropyModel*)m;
}

void ArithmeticDecoder::initBitModel(EntropyModel* model)
{
  ArithmeticBitModel* m = (ArithmeticBitModel*)model;
  m->init();
}

void ArithmeticDecoder::destroyBitModel(EntropyModel* model)
{
  ArithmeticBitModel* m = (ArithmeticBitModel*)model;
  delete m;
}

EntropyModel* ArithmeticDecoder::createSymbolModel(U32 n)
{
  ArithmeticModel* m = new ArithmeticModel(n, false);
  return (EntropyModel*)m;
}

void ArithmeticDecoder::initSymbolModel(EntropyModel* model, U32 *table)
{
  ArithmeticModel* m = (ArithmeticModel*)model;
  m->init(table);
}

void ArithmeticDecoder::destroySymbolModel(EntropyModel* model)
{
  ArithmeticModel* m = (ArithmeticModel*)model;
  delete m;
}

U32 ArithmeticDecoder::decodeBit(EntropyModel* model)
{
  ArithmeticBitModel* m = (ArithmeticBitModel*)model;
  U32 x = m->bit_0_prob * (length >> BM__LengthShift);       // product l x p0
  U32 sym = (value >= x);                                          // decision
                                                    // update & shift interval
  if (sym == 0) {
    length  = x;
    ++m->bit_0_count;
  }
  else {
    value  -= x;                                  // shifted interval base = 0
    length -= x;
  }

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization
  if (--m->bits_until_update == 0) m->update();       // periodic model update

  return sym;                                         // return data bit value
}

U32 ArithmeticDecoder::decodeSymbol(EntropyModel* model)
{
  ArithmeticModel* m = (ArithmeticModel*)model;
  U32 n, sym, x, y = length;

  if (m->decoder_table) {             // use table look-up for faster decoding

    unsigned dv = value / (length >>= DM__LengthShift);
    unsigned t = dv >> m->table_shift;

    sym = m->decoder_table[t];      // initial decision based on table look-up
    n = m->decoder_table[t+1] + 1;

    while (n > sym + 1) {                      // finish with bisection search
      U32 k = (sym + n) >> 1;
      if (m->distribution[k] > dv) n = k; else sym = k;
    }
                                                           // compute products
    x = m->distribution[sym] * length;
    if (sym != m->last_symbol) y = m->distribution[sym+1] * length;
  }

  else {                                  // decode using only multiplications

    x = sym = 0;
    length >>= DM__LengthShift;
    U32 k = (n = m->symbols) >> 1;
                                                // decode via bisection search
    do {
      U32 z = length * m->distribution[k];
      if (z > value) {
        n = k;
        y = z;                                             // value is smaller
      }
      else {
        sym = k;
        x = z;                                     // value is larger or equal
      }
    } while ((k = (sym + n) >> 1) != sym);
  }

  value -= x;                                               // update interval
  length = y - x;

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  ++m->symbol_count[sym];
  if (--m->symbols_until_update == 0) m->update();    // periodic model update

  return sym;
}

U32 ArithmeticDecoder::readBit()
{
  U32 sym = value / (length >>= 1);            // decode symbol, change length
  value -= length * sym;                                    // update interval

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  return sym;
}

U32 ArithmeticDecoder::readBits(U32 bits)
{
  assert(bits && (bits <= 32));

  if (bits > 19)
  {
    U32 tmp = readShort();
    bits = bits - 16;
    U32 tmp1 = readBits(bits) << 16;
    return (tmp1|tmp);
  }

  U32 sym = value / (length >>= bits);// decode symbol, change length
  value -= length * sym;                                    // update interval

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  return sym;
}

U8 ArithmeticDecoder::readByte()
{
  U32 sym = value / (length >>= 8);            // decode symbol, change length
  value -= length * sym;                                    // update interval

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  assert(sym < (1<<8));

  return (U8)sym;
}

unsigned short ArithmeticDecoder::readShort()
{
  U32 sym = value / (length >>= 16);           // decode symbol, change length
  value -= length * sym;                                    // update interval

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  assert(sym < (1<<16));

  return (U16)sym;
}

inline U32 ArithmeticDecoder::readInt()
{
  U32 lowerInt = readShort();
  U32 upperInt = readShort();
  return upperInt*U16_MAX_PLUS_ONE+lowerInt;
}

inline F32 ArithmeticDecoder::readFloat()
{
  U32F32 u32f32;
  u32f32.u32 = readInt();
  return u32f32.f32;
}

inline U64 ArithmeticDecoder::readInt64()
{
  U64 lowerInt = readInt();
  U64 upperInt = readInt();
  return upperInt*U32_MAX_PLUS_ONE+lowerInt;
}

inline F64 ArithmeticDecoder::readDouble()
{
  U64F64 u64f64;
  u64f64.u64 = readInt64();
  return u64f64.f64;
}

ArithmeticDecoder::~ArithmeticDecoder()
{

}

inline void ArithmeticDecoder::renorm_dec_interval()
{
  do {                                          // read least-significant byte
    value = (value << 8) | instream->getByte();
  } while ((length <<= 8) < AC__MinLength);        // length multiplied by 256
}
