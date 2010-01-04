/******************************************************************************
 *
 * Project:  laszip - http://liblas.org - 
 * Purpose:  
 * Author:   Martin Isenburg
 *           martin.isenburg at gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2009, Martin Isenburg
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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


    
#include "arithmeticdecoder.h"


namespace laszipalternate {
    
ArithmeticDecoder::ArithmeticDecoder(unsigned char* bytes, unsigned int number_bytes)
{
  inbuffer = bytes;
  endbuffer = bytes + number_bytes;
  fp = 0;

  /*
  if ((value = get_inbyte()) != AC_HEADER_BYTE)
  {
    fprintf(stderr, "ArithmeticDecoder: wrong AC_HEADER_BYTE of %d. is should be %d\n", value, AC_HEADER_BYTE);
    return;
  }
  */

  length = AC__MaxLength;
  value = (get_inbyte() << 24);
  value |= (get_inbyte() << 16);
  value |= (get_inbyte() << 8);
  value |= (get_inbyte());
}

ArithmeticDecoder::ArithmeticDecoder(FILE* fp)
{
  inbuffer = 0;
  endbuffer = 0;
  this->fp = fp;

/*
  if ((value = get_inbyte()) != AC_HEADER_BYTE)
  {
    fprintf(stderr, "ArithmeticDecoder: wrong AC_HEADER_BYTE of %d. is should be %d\n", value, AC_HEADER_BYTE);
    return;
  }
*/

  length = AC__MaxLength;
  value = (get_inbyte() << 24);
  value |= (get_inbyte() << 16);
  value |= (get_inbyte() << 8);
  value |= (get_inbyte());
}

unsigned int ArithmeticDecoder::decode(ArithmeticBitModel* m)
{
  unsigned int x = m->bit_0_prob * (length >> BM__LengthShift); // product l x p0
  unsigned int sym = (value >= x);                                 // decision
                                                    // update & shift interval
  if (sym == 0) {
    length  = x;
    ++m->bit_0_count;
  }
  else {
    value  -= x;                                 // shifted interval base = 0
    length -= x;
  }

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization
  if (--m->bits_until_update == 0) m->update();       // periodic model update

  return sym;                                         // return data bit value
}

unsigned int ArithmeticDecoder::decode(ArithmeticModel* m)
{
  unsigned n, sym, x, y = length;

  if (m->decoder_table) {              // use table look-up for faster decoding

    unsigned dv = value / (length >>= DM__LengthShift);
    unsigned t = dv >> m->table_shift;

    sym = m->decoder_table[t];         // initial decision based on table look-up
    n = m->decoder_table[t+1] + 1;

    while (n > sym + 1) {                        // finish with bisection search
      unsigned int k = (sym + n) >> 1;
      if (m->distribution[k] > dv) n = k; else sym = k;
    }
                                                           // compute products
    x = m->distribution[sym] * length;
    if (sym != m->last_symbol) y = m->distribution[sym+1] * length;
  }

  else {                                  // decode using only multiplications

    x = sym = 0;
    length >>= DM__LengthShift;
    unsigned int k = (n = m->data_symbols) >> 1;
                                                // decode via bisection search
    do {
      unsigned int z = length * m->distribution[k];
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
  if (--m->symbols_until_update == 0) m->update(false); // periodic model update

  return sym;
}

unsigned int ArithmeticDecoder::readBits(unsigned int bits)
{
  assert(bits && (bits <= 32));

  if (bits > 20)
  {
    unsigned int tmp = readShort();
    bits = bits - 16;
    unsigned int tmp1 = readBits(bits) << 16;
    return (tmp1|tmp);
  }

  unsigned int sym = value / (length >>= bits);// decode symbol, change length
  value -= length * sym;                                    // update interval

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  return sym;
}

unsigned char ArithmeticDecoder::readByte()
{
  unsigned int sym = value / (length >>= 8);   // decode symbol, change length
  value -= length * sym;                                    // update interval

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  assert(sym < (1<<8));

  return (unsigned char)sym;
}

unsigned short ArithmeticDecoder::readShort()
{
  unsigned int sym = value / (length >>= 16);  // decode symbol, change length
  value -= length * sym;                                    // update interval

  if (length < AC__MinLength) renorm_dec_interval();        // renormalization

  assert(sym < (1<<16));

  return (unsigned short)sym;
}

unsigned int ArithmeticDecoder::readInt()
{
  unsigned int lowerInt = readShort();
  unsigned int upperInt = readShort();
  return upperInt*65536+lowerInt;
}

float ArithmeticDecoder::readFloat()
{
  float f;
  *((unsigned int*)(&f)) = readInt();
  return f;
}

U64 ArithmeticDecoder::readInt64()
{
  U64 lowerInt = readInt();
  U64 upperInt = readInt();
  return upperInt*4294967296+lowerInt;
}

double ArithmeticDecoder::readDouble()
{
  double d;
  *((U64*)(&d)) = readInt64();
  return d;
}

void ArithmeticDecoder::done()
{
}

ArithmeticDecoder::~ArithmeticDecoder()
{
}

inline void ArithmeticDecoder::renorm_dec_interval()
{
  do {                                          // read least-significant byte
    value = (value << 8) | get_inbyte();
  } while ((length <<= 8) < AC__MinLength);        // length multiplied by 256
}

int read_bytes = 0;

inline unsigned int ArithmeticDecoder::get_inbyte()
{
  int c;
  if (fp)
  {
    read_bytes++;
    c = getc(fp);
    if (c == EOF)
    {
      c = 0;
    }
  }
  else
  {
    if (inbuffer == endbuffer)
    {
      c = 0;
    }
    else
    {
      c = *inbuffer++;
    }
  }
  return c;
}

}