/*
===============================================================================

  FILE:  rangeencoder.cpp
  
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
#include "rangeencoder.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

RangeEncoder::RangeEncoder(FILE* fp, bool store_chars)
{
  if (fp)
  {
    this->fp = fp;
    chars = 0;
    number_chars = 0;
  }
  else
  {
    this->fp = 0;
    if (store_chars)
    {
      chars = (U8*)malloc(sizeof(U8)*1000);
      number_chars = 0;
      allocated_chars = 1000;
    }
    else
    {
      chars = 0;
      number_chars = 0;
    }
  }
  low = 0;                /* Full code range */
  range = TOP_VALUE;
  /* this buffer is written as first byte in the datastream (header,...) */
  buffer = HEADERBYTE;
  help = 0;               /* No bytes to follow */
  bytecount = 0;
}

void RangeEncoder::encode(RangeModel* rm, U32 sym)
{
  U32 syfreq;
  U32 ltfreq;
  U32 r, tmp;
  U32 lg_totf = rm->lg_totf;

  assert(sym >= 0 && sym < (U32)rm->n);
  
  rm->getfreq(sym,&syfreq,&ltfreq);

  normalize();
  r = range >> lg_totf;
  tmp = r * ltfreq;
  low += tmp;
#ifdef EXTRAFAST
  range = r * syfreq;
#else
  if ((ltfreq+syfreq) >> lg_totf)
  {
    range -= tmp;
  }
  else
  {
    range = r * syfreq;
  }
#endif

  rm->update(sym);
}

void RangeEncoder::writeBit(U32 sym)
{
  assert(sym < 2);

  U32 r, tmp;
  normalize();
  r = range >> 1;
  tmp = r * sym;
  low += tmp;
#ifdef EXTRAFAST
  range = r;
#else
  if ((sym+1) >> 1)
  {
    range -= tmp;
  }
  else
  {
    range = r;
  }
#endif
}

void RangeEncoder::writeBits(U32 bits, U32 sym)
{
  assert(bits && (sym < (1u<<bits)));

  if (bits > 21) // 22 bits
  {
    writeShort(sym&U16_MAX);
    sym = sym >> 16;
    bits = bits - 16;
  }

  U32 r, tmp;
  normalize();
  r = range >> bits;
  tmp = r * sym;
  low += tmp;
#ifdef EXTRAFAST
  range = r;
#else
  if ((sym+1) >> bits)
  {
    range -= tmp;
  }
  else
  {
    range = r;
  }
#endif
}

void RangeEncoder::writeRange(U32 range, U32 sym)
{
  assert(range && (sym < range));

  if (range > 4194303) // 22 bits
  {
    writeShort(sym&U16_MAX);
    sym = sym >> 16;
    range = range >> 16;
    range++;
  }
  U32 r, tmp;
  normalize();
  r = this->range / range;
  tmp = r * sym;
  low += tmp;
#ifdef EXTRAFAST
  this->range = r;
#else
  if (sym+1 < range)
  {
    this->range = r;
  }
  else
  {
    this->range -= tmp;
  }
#endif
}

void RangeEncoder::writeRange64(U64 range, U64 sym)
{
  assert(sym < range);
  if (range > U32_MAX) // 32 bits
  {
    writeInt((U32)(sym&U32_MAX));
    sym = sym >> 32;
    range = range >> 32;
    range++;
  }
  writeRange((U32)range, (U32)sym);
}

void RangeEncoder::writeByte(U8 sym)
{
  U32 r, tmp;
  normalize();
  r = range >> 8;
  tmp = r * (U32)(sym);
  low += tmp;
#ifdef EXTRAFAST
  range = r;
#else
  if (((U32)(sym)+1) >> 8)
  {
    range -= tmp;
  }
  else
  {
    range = r;
  }
#endif
}

void RangeEncoder::writeShort(U16 sym)
{
  U32 r, tmp;
  normalize();
  r = range >> 16;
  tmp = r * (U32)(sym);
  low += tmp;
#ifdef EXTRAFAST
  range = r;
#else
  if (((U32)(sym)+1) >> 16)
  {
    range -= tmp;
  }
  else
  {
    range = r;
  }
#endif
}

void RangeEncoder::writeInt(U32 sym)
{
  writeShort((U16)(sym % U16_MAX_PLUS_ONE)); // lower 16 bits
  writeShort((U16)(sym / U16_MAX_PLUS_ONE)); // UPPER 16 bits
}

void RangeEncoder::writeInt64(U64 sym)
{
  writeInt((U32)(sym % U32_MAX_PLUS_ONE)); // lower 32 bits
  writeInt((U32)(sym / U32_MAX_PLUS_ONE)); // UPPER 32 bits
}

void RangeEncoder::writeFloat(F32 sym)
{
  U32F32 u32f32;
  u32f32.f32 = sym;
  writeInt(u32f32.u32);
}

void RangeEncoder::writeDouble(F64 sym)
{
  U64F64 u64f64;
  u64f64.f64 = sym;
  writeInt64(u64f64.u64);
}

/* I do the normalization before I need a defined state instead of */
/* after messing it up. This simplifies starting and ending.       */
inline void RangeEncoder::normalize()
{
  while(range <= BOTTOM_VALUE) /* do we need renormalisation?  */
  {
    if (low < (U32)0xff<<SHIFT_BITS)  /* no carry possible --> output */
    {
      outbyte(buffer);
      for(; help; help--)
      {
        outbyte(0xff);
      }
      buffer = (U8)(low >> SHIFT_BITS);
    }
    else if (low & TOP_VALUE) /* carry now, no future carry */
    {
      outbyte(buffer+1);
      for(; help; help--)
      {
        outbyte(0);
      }
      buffer = (U8)(low >> SHIFT_BITS);
    }
    else                      /* passes on a potential carry */
    {
      help++;
    }
    range <<= 8;
    low = (low<<8) & (TOP_VALUE-1);
    bytecount++;
  }
}

/* Finish encoding                                           */
/* actually not that many bytes need to be output, but who   */
/* cares. I output them because decode will read them :)     */
/* the return value is the number of bytes written           */
U32 RangeEncoder::done()
{
  U32 tmp;
  normalize();     /* now we have a normalized state */
  bytecount += 5;
  if ((low & (BOTTOM_VALUE-1)) < ((bytecount&0xffffffL)>>1))
  {
    tmp = low >> SHIFT_BITS;
  }
  else
  {
    tmp = (low >> SHIFT_BITS) + 1;
  }
  if (tmp > 0xff) /* we have a carry */
  {
    outbyte(buffer+1);
    for(; help; help--)
    {
      outbyte(0);
    }
  }
  else  /* no carry */
  {
    outbyte(buffer);
    for(; help; help--)
    {
      outbyte(0xff);
    }
  }
  outbyte(tmp & 0xff);
  outbyte((bytecount>>16) & 0xff);
  outbyte((bytecount>>8) & 0xff);
  outbyte(bytecount & 0xff);
  return bytecount;
}

RangeEncoder::~RangeEncoder()
{
  if (chars)
  {
    free(chars);
  }
}

U8* RangeEncoder::getChars()
{
  return chars;
}
  
U32 RangeEncoder::getNumberChars()
{
  return number_chars;
}

U32 RangeEncoder::getNumberBytes()
{
  return bytecount;
}

inline void RangeEncoder::outbyte(U32 c)
{
  if (fp)
  {
    fputc(c, fp);
  }
  else
  {
    if (chars)
    {
      if (number_chars == allocated_chars)
      {
        U8* newchars = (U8*)malloc(sizeof(U8)*allocated_chars*2);
        memcpy(newchars,chars,sizeof(U8)*allocated_chars);
        free(chars);
        chars = newchars;
        allocated_chars = allocated_chars*2;
      }
      chars[number_chars++] = c;
    }
  }
}
