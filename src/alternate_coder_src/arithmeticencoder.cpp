/******************************************************************************
 *
 * Project:  laszip - http://liblas.org -
 * Purpose:
 * Author:   Martin Isenburg
 *           isenburg at cs.unc.edu
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

  FILE:  arithmeticencoder.cpp
  
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
#include "arithmeticencoder.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


namespace laszipalternate {
    
ArithmeticEncoder::ArithmeticEncoder(FILE* fp, int store_bytes)
{
  if (fp || store_bytes == 0)
  {
    this->fp = fp;
    this->store_bytes = false;
    outbuffer = (unsigned char*)malloc(sizeof(unsigned char)*2*AC_BUFFER_SIZE);
    endbuffer = outbuffer + 2 * AC_BUFFER_SIZE;
  }
  else
  {
    this->fp = 0;
    this->store_bytes = true;
    outbuffer = (unsigned char*)malloc(sizeof(unsigned char)*store_bytes);
    endbuffer = outbuffer + store_bytes;
  }
  this->fp = fp;
  outbyte = outbuffer;
  endbyte = endbuffer;
  bytes_output = 0;
  base   = 0;
  length = AC__MaxLength;
}

void ArithmeticEncoder::encode(ArithmeticBitModel* m, unsigned int sym)
{
  assert(m && (sym <= 1));

  unsigned int x = m->bit_0_prob * (length >> BM__LengthShift); // product l x p0
                                                            // update interval
  if (sym == 0) {
    length = x;
    ++m->bit_0_count;
  }
  else {
    unsigned int init_base = base;
    base += x;
    length -= x;
    if (init_base > base) propagate_carry();               // overflow = carry
  }

  if (length < AC__MinLength) renorm_enc_interval();        // renormalization
  if (--m->bits_until_update == 0) m->update();       // periodic model update
}

void ArithmeticEncoder::encode(ArithmeticModel* m, unsigned int sym)
{
  assert(m && (sym <= m->last_symbol));

  unsigned int x, init_base = base;
                                                            // compute products
  if (sym == m->last_symbol) {
    x = m->distribution[sym] * (length >> DM__LengthShift);
    base   += x;                                             // update interval
    length -= x;                                           // no product needed
  }
  else {
    x = m->distribution[sym] * (length >>= DM__LengthShift);
    base   += x;                                             // update interval
    length  = m->distribution[sym+1] * length - x;
  }

  if (init_base > base) propagate_carry();                  // overflow = carry
  if (length < AC__MinLength) renorm_enc_interval();         // renormalization

  ++m->symbol_count[sym];
  if (--m->symbols_until_update == 0) m->update(true); // periodic model update
}

void ArithmeticEncoder::writeBits(unsigned int bits, unsigned int sym)
{
  assert(bits && (bits <= 32) && (sym < (1u<<bits)));

  if (bits > 19)
  {
    writeShort(sym&65535);
    sym = sym >> 16;
    bits = bits - 16;
  }

  unsigned init_base = base;
  base += sym * (length >>= bits);            // new interval base and length

  if (init_base > base) propagate_carry();                 // overflow = carry
  if (length < AC__MinLength) renorm_enc_interval();        // renormalization
}

void ArithmeticEncoder::writeByte(unsigned char c)
{
  unsigned int init_base = base;
  base += (unsigned int)(c) * (length >>= 8);            // new interval base and length

  if (init_base > base) propagate_carry();                 // overflow = carry
  if (length < AC__MinLength) renorm_enc_interval();        // renormalization
}

void ArithmeticEncoder::writeShort(unsigned short s)
{
  unsigned int init_base = base;
  base += (unsigned int)(s) * (length >>= 16);            // new interval base and length

  if (init_base > base) propagate_carry();                 // overflow = carry
  if (length < AC__MinLength) renorm_enc_interval();        // renormalization
}

void ArithmeticEncoder::writeInt(unsigned int i)
{
  writeShort((unsigned short)(i % 65536)); // lower 16 bits
  writeShort((unsigned short)(i / 65536)); // UPPER 16 bits
}

void ArithmeticEncoder::writeInt64(U64 l)
{
  writeInt((unsigned int)(l % 4294967296)); // lower 32 bits
  writeInt((unsigned int)(l / 4294967296)); // UPPER 32 bits
}

void ArithmeticEncoder::writeFloat(float f)
{
  writeInt(*((unsigned int*)(&f)));
}

void ArithmeticEncoder::writeDouble(double d)
{
  writeInt64(*((U64*)(&d)));
}

unsigned int ArithmeticEncoder::done()
{
  unsigned int init_base = base;        // done encoding: set final data bytes

  if (length > 2 * AC__MinLength) {
    base  += AC__MinLength;                                     // base offset
    length = AC__MinLength >> 1;             // set new length for 1 more byte
  }
  else {
    base  += AC__MinLength >> 1;                                // base offset
    length = AC__MinLength >> 9;            // set new length for 2 more bytes
  }

  if (init_base > base) propagate_carry();                 // overflow = carry
  renorm_enc_interval();                // renormalization = output last bytes

  if (fp)
  {
    if (endbyte != endbuffer)
    {
      assert(outbyte < outbuffer + AC_BUFFER_SIZE);
      fwrite(outbuffer + AC_BUFFER_SIZE, 1, AC_BUFFER_SIZE, fp);
      bytes_output += AC_BUFFER_SIZE;
    }
    unsigned int buffer_size = outbyte - outbuffer;
    fwrite(outbuffer, 1, buffer_size, fp);
    bytes_output += buffer_size;
  }
  else if (store_bytes == false)
  {
    bytes_output += (outbyte - outbuffer);
  }

  return getNumberBytes();                             // number of bytes used
}

ArithmeticEncoder::~ArithmeticEncoder()
{
//  free(outbuffer);
}

unsigned char* ArithmeticEncoder::getBytes()
{
  if (store_bytes)
    return outbuffer;
  else
    return 0;
}
  
unsigned int ArithmeticEncoder::getNumberBytes()
{
  if (store_bytes)
    return outbyte - outbuffer;
  else
    return bytes_output;
}

int max_carry = 0;

inline void ArithmeticEncoder::propagate_carry()
{
  int carry = 0;
  unsigned char * p;
  if (outbyte == outbuffer)
    p = endbuffer - 1;
  else
    p = outbyte - 1;
  while (*p == 0xFFU)
  {
    *p = 0;
    if (p == outbuffer)
      p = endbuffer - 1;
    else
      p--;
    assert(outbuffer <= p);
    assert(p < endbuffer);    
    assert(outbyte < endbuffer);    
    carry++;
  }
  ++*p;
  if (carry > max_carry) max_carry = carry;
}

inline void ArithmeticEncoder::renorm_enc_interval()
{
  do {                                          // output and discard top byte
    assert(outbuffer <= outbyte);
    assert(outbyte < endbuffer);    
    assert(outbyte < endbyte);    
    *outbyte++ = (unsigned char)(base >> 24);
    if (outbyte == endbyte) manage_outbuffer();
    base <<= 8;
  } while ((length <<= 8) < AC__MinLength);        // length multiplied by 256
}

inline void ArithmeticEncoder::manage_outbuffer()
{
  if (fp)
  {
    if (outbyte == endbuffer) outbyte = outbuffer;
    fwrite(outbyte, 1, AC_BUFFER_SIZE, fp);
    bytes_output += AC_BUFFER_SIZE;
    endbyte = outbyte + AC_BUFFER_SIZE;
    assert(endbyte > outbyte);
    assert(outbyte < endbuffer);    
  }
  else if (store_bytes)
  {
    unsigned int buffer_size = endbyte - outbuffer;
    unsigned char* newbuffer = (unsigned char*) malloc(sizeof(unsigned char)*buffer_size*2);
    memcpy(newbuffer,outbuffer,sizeof(unsigned char)*buffer_size);
    free(outbuffer);
    outbuffer = newbuffer;
    endbuffer = newbuffer + buffer_size * 2;
    outbyte = outbuffer + buffer_size;
    endbyte = endbuffer;
  }
  else
  {
    bytes_output += 2 * AC_BUFFER_SIZE;
    outbyte = outbuffer;
  }
}

}