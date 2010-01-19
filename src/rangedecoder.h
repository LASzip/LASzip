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

  FILE:  rangedecoder.h
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    Copyright (C) 2003 Martin Isenburg (isenburg@cs.unc.edu)
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    14 January 2003 -- adapted from michael schindler's code before SIGGRAPH
  
===============================================================================
*/
#ifndef RANGEDECODER_H
#define RANGEDECODER_H

#include <stdio.h>

#include "rangemodel.h"

class RangeDecoder
{
public:

/* Start the decoder                                         */
  RangeDecoder(U8* chars, U32 number_chars);
  RangeDecoder(FILE* fp);

  ~RangeDecoder();

/* Decode with modelling                                     */
  U32 decode(RangeModel* rm);

/* Decode a bit without modelling                            */
  U32 readBit();

/* Decode bits without modelling                             */
  U32 readBits(U32 bits);

/* Decode a range without modelling                          */
  U32 readRange(U32 range);

/* Decode a range without modelling                          */
  U64 readRange64(U64 range);

/* Decode an unsigned char without modelling                 */
  U8 readByte();

/* Decode an unsigned short without modelling                */
  U16 readShort();

/* Decode an unsigned int without modelling                  */
  U32 readInt();

/* Decode a float without modelling                          */
  F32 readFloat();

/* Decode an unsigned 64 bit int without modelling           */
  U64 readInt64();

/* Decode a double without modelling                         */
  double readDouble();

/* Finish decoding                                           */
  void done();

private:
/* Calculate culmulative frequency for next symbol, no update*/
/* tot_f is the total frequency                              */
/* or: totf is 1<<shift                                      */
/* returns the <= culmulative frequency                      */
  U32 culshift(U32 shift);

/* Update decoding state                                     */
/* sy_f is the interval length (frequency of the symbol)     */
/* lt_f is the lower end (frequency sum of < symbols)        */
/* tot_f is the total interval length (total frequency sum)  */
  void update(U32 sy_f, U32 lt_f, U32 tot_f);

  inline void normalize();
  inline U32 inbyte();

  FILE* fp;

  U8* chars;
  U32 current_char;
  U32 number_chars;

  U32 low;         /* low end of interval */
  U32 range;       /* length of interval */
  U32 help;        /* bytes_to_follow resp. intermediate value */
  U8 buffer;       /* buffer for output */
};

#endif
