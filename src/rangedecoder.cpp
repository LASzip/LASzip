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

/*
===============================================================================

  FILE:  rangedecoder.cpp
  
  CONTENTS:
      
    see header file

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2003 martin isenburg (isenburg@cs.unc.edu)
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see header file
  
===============================================================================
*/
#include "rangedecoder.hpp"

inline void RangeDecoder::normalize()
{
  while (range <= BOTTOM_VALUE)
  {
    low = (low<<8) | ((buffer<<EXTRA_BITS)&0xff);
    buffer = inbyte();
    low |= buffer >> (8-EXTRA_BITS);
    range <<= 8;
  }
}

U32 RangeDecoder::decode(RangeModel* rm)
{
  U32 sym;
  U32 ltfreq;
  U32 syfreq;
  U32 tmp;
  U32 lg_totf = rm->lg_totf;

  normalize();
  help = this->range>>lg_totf;
  ltfreq = low/help;
#ifdef EXTRAFAST
  ltfreq = ltfreq;
#else
  ltfreq = ((ltfreq>>lg_totf) ? (1<<lg_totf)-1 : ltfreq);
#endif

  sym = rm->getsym(ltfreq);
  rm->getfreq(sym,&syfreq,&ltfreq);

  tmp = help * ltfreq;
  low -= tmp;
#ifdef EXTRAFAST
  this->range = help * syfreq;
#else
  if ((ltfreq + syfreq) < (1u<<lg_totf))
  {
    this->range = help * syfreq;
  }
  else
  {
    this->range -= tmp;
  }
#endif

  rm->update(sym);

  return sym;
}

U32 RangeDecoder::readBit()
{
  U32 tmp;
  tmp = culshift(1);
  update(1, tmp, 2);
  return tmp;
}

U32 RangeDecoder::readBits(U32 bits)
{
  U32 tmp;
  if (bits > 21) // 22 bits
  {
    tmp = readShort();
    U32 tmp1 = readBits(bits - 16) << 16;
    return (tmp1|tmp);
  }
  tmp = culshift(bits);
  update(1, tmp, 1u<<bits);
  return tmp;
}

U32 RangeDecoder::readRange(U32 range)
{
  U32 tmp;
  U32 tmp1;

  if (range > 4194303) // 22 bits
  {
    tmp = readShort();
    range = range >> 16;
    range++;
    tmp1 = readRange(range) << 16;
    return (tmp1|tmp);
  }
  
  normalize();
  help = this->range/range;
  tmp = low/help;
#ifdef EXTRAFAST
  tmp = tmp;
#else
  tmp = (tmp>=range ? range-1 : tmp);
#endif

  tmp1 = (help * tmp);
  low -= tmp1;
#ifdef EXTRAFAST
  this->range = help;
#else
  if (tmp+1 < range)
  {
    this->range = help;
  }
  else
  {
    this->range -= tmp1;
  }
#endif

  return tmp;
}

/* Decode a range without modelling                          */
U64 RangeDecoder::readRange64(U64 range)
{
  if (range > U32_MAX) // 32 bits
  {
    U64 tmp;
    U64 tmp1;
    tmp = readInt();
    range = range >> 32;
    range++;
    tmp1 = ((U64)(readRange((U32)(range)))) << 32;
    return (tmp1|tmp);
  }
  else
  {
    return (U64)(readRange((U32)(range)));
  }
}

/* Decode a byte without modelling                           */
U8 RangeDecoder::readByte()
{
  U8 tmp = culshift(8);
  update(1, tmp, 1u<<8);
  return tmp;
}

/* Decode a short without modelling                          */
U16 RangeDecoder::readShort()
{
  unsigned short tmp = culshift(16);
  update(1, tmp, 1u<<16);
  return tmp;
}

/* Decode an unsigned int without modelling                  */
U32 RangeDecoder::readInt()
{
  U32 lowerInt = readShort();
  U32 upperInt = readShort();
  return upperInt*U16_MAX_PLUS_ONE+lowerInt;
}

/* Decode a float without modelling                          */
F32 RangeDecoder::readFloat()
{
  U32F32 u32f32;
  u32f32.u32 = readInt();
  return u32f32.f32;
}

/* Decode an unsigned 64 bit int without modelling           */
U64 RangeDecoder::readInt64()
{
  U64 lowerInt = readInt();
  U64 upperInt = readInt();
  return upperInt*U32_MAX_PLUS_ONE+lowerInt;
}

/* Decode a double without modelling                         */
F64 RangeDecoder::readDouble()
{
  U64F64 u64f64;
  u64f64.u64 = readInt64();
  return u64f64.f64;
}

/* Finish decoding                                           */
void RangeDecoder::done()
{
  normalize();      /* normalize to use up all bytes */
}

U32 RangeDecoder::culshift(U32 shift)
{
  U32 tmp;
  normalize();
  help = range>>shift;
  tmp = low/help;
#ifdef EXTRAFAST
  return tmp;
#else
  return (tmp>>shift ? (1u<<shift)-1 : tmp);
#endif
}

/* Update decoding state                                     */
/* sy_f is the interval length (frequency of the symbol)     */
/* lt_f is the lower end (frequency sum of < symbols)        */
/* tot_f is the total interval length (total frequency sum)  */
void RangeDecoder::update(U32 sy_f, U32 lt_f, U32 tot_f)
{
  U32 tmp;
  tmp = help * lt_f;
  low -= tmp;
#ifdef EXTRAFAST
  this->range = help * sy_f;
#else
  if (lt_f + sy_f < tot_f)
  {
    this->range = help * sy_f;
  }
  else
  {
    this->range -= tmp;
  }
#endif
}

RangeDecoder::RangeDecoder(U8* chars, U32 number_chars)
{
  this->chars = chars;
  this->number_chars = number_chars;
  current_char = 0;
  fp = 0;

  buffer = inbyte();
  if (buffer != HEADERBYTE)
  {
    fprintf(stderr, "RangeDecoder: wrong HEADERBYTE of %d. is should be %d\n", buffer, HEADERBYTE);
    return;
  }
  buffer = inbyte();
  low = buffer >> (8-EXTRA_BITS);
  range = (U32)1 << EXTRA_BITS;
}

RangeDecoder::RangeDecoder(FILE* fp)
{
  chars = 0;
  number_chars = 0;
  current_char = 0;
  this->fp = fp;

  buffer = inbyte();
  if (buffer != HEADERBYTE)
  {
    fprintf(stderr, "RangeDecoder: wrong HEADERBYTE of %d. is should be %d\n", buffer, HEADERBYTE);
    return;
  }
  buffer = inbyte();
  low = buffer >> (8-EXTRA_BITS);
  range = (U32)1 << EXTRA_BITS;
}

RangeDecoder::~RangeDecoder()
{
}

inline U32 RangeDecoder::inbyte()
{
  U32 c;
  if (fp)
  {
    c = getc(fp);
  }
  else
  {
    if (current_char < number_chars)
    {
      c = chars[current_char++];
    }
    else
    {
      c = (U32)EOF;
    }
  }
  return c;
}
