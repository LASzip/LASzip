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
      chars = (unsigned char*)malloc(sizeof(unsigned char)*1000);
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

void RangeEncoder::encode(RangeModel* rm, unsigned int sym)
{
  unsigned int syfreq;
  unsigned int ltfreq;
  unsigned int r, tmp;
  unsigned int lg_totf = rm->lg_totf;

  assert(sym >= 0 && sym < (unsigned int)rm->n);
  
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

void RangeEncoder::writeBits(unsigned int bits, unsigned int sym)
{
  assert(bits && (sym < (1u<<bits)));

  if (bits > 21) // 22 bits
  {
    writeShort(sym&65535);
    sym = sym >> 16;
    bits = bits - 16;
  }

  unsigned int r, tmp;
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

void RangeEncoder::writeRange(unsigned int range, unsigned int sym)
{
  assert(range && (sym < range));

  if (range > 4194303) // 22 bits
  {
    writeShort(sym&65535);
    sym = sym >> 16;
    range = range >> 16;
    range++;
  }
  unsigned int r, tmp;
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
  if (range > 4294967295) // 32 bits
  {
    writeInt((unsigned int)(sym&4294967295));
    sym = sym >> 32;
    range = range >> 32;
    range++;
  }
  writeRange((unsigned int)range, (unsigned int)sym);
}

void RangeEncoder::writeByte(unsigned char c)
{
  unsigned int r, tmp;
  normalize();
  r = range >> 8;
  tmp = r * (unsigned int)(c);
  low += tmp;
#ifdef EXTRAFAST
  range = r;
#else
  if (((unsigned int)(c)+1) >> 8)
  {
    range -= tmp;
  }
  else
  {
    range = r;
  }
#endif
}

void RangeEncoder::writeShort(unsigned short s)
{
  unsigned int r, tmp;
  normalize();
  r = range >> 16;
  tmp = r * (unsigned int)(s);
  low += tmp;
#ifdef EXTRAFAST
  range = r;
#else
  if (((unsigned int)(s)+1) >> 16)
  {
    range -= tmp;
  }
  else
  {
    range = r;
  }
#endif
}

void RangeEncoder::writeInt(unsigned int i)
{
  writeShort((unsigned short)(i % 65536)); // lower 16 bits
  writeShort((unsigned short)(i / 65536)); // UPPER 16 bits
}

void RangeEncoder::writeInt64(U64 l)
{
  writeInt((unsigned int)(l % 4294967296ull)); // lower 32 bits
  writeInt((unsigned int)(l / 4294967296ull)); // UPPER 32 bits
}

void RangeEncoder::writeFloat(float f)
{
  writeInt(*((unsigned int*)(&f)));
}

void RangeEncoder::writeDouble(double d)
{
  writeInt64(*((U64*)(&d)));
}

/* I do the normalization before I need a defined state instead of */
/* after messing it up. This simplifies starting and ending.       */
inline void RangeEncoder::normalize()
{
  while(range <= BOTTOM_VALUE) /* do we need renormalisation?  */
  {
    if (low < (unsigned int)0xff<<SHIFT_BITS)  /* no carry possible --> output */
    {
      outbyte(buffer);
      for(; help; help--)
      {
        outbyte(0xff);
      }
      buffer = (unsigned char)(low >> SHIFT_BITS);
    }
    else if (low & TOP_VALUE) /* carry now, no future carry */
    {
      outbyte(buffer+1);
      for(; help; help--)
      {
        outbyte(0);
      }
      buffer = (unsigned char)(low >> SHIFT_BITS);
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
unsigned int RangeEncoder::done()
{
  unsigned int tmp;
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

unsigned char* RangeEncoder::getChars()
{
  return chars;
}
  
int RangeEncoder::getNumberChars()
{
  return number_chars;
}

long RangeEncoder::getNumberBits()
{
  return bytecount*8;
}

int RangeEncoder::getNumberBytes()
{
  return bytecount;
}

inline void RangeEncoder::outbyte(unsigned int c)
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
        unsigned char* newchars = (unsigned char*) malloc(sizeof(unsigned char)*allocated_chars*2);
        memcpy(newchars,chars,sizeof(unsigned char)*allocated_chars);
        free(chars);
        chars = newchars;
        allocated_chars = allocated_chars*2;
      }
      chars[number_chars++] = c;
    }
  }
}
