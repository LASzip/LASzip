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
#include "mydefs.h"

class RangeDecoder
{
public:

/* Start the decoder                                         */
  RangeDecoder(unsigned char* chars, int number_chars);
  RangeDecoder(FILE* fp);

  ~RangeDecoder();

/* Decode with modelling                                     */
  unsigned int decode(RangeModel* rm);

/* Decode bits without modelling                             */
  unsigned int readBits(unsigned int bits);

/* Decode a range without modelling                          */
  unsigned int readRange(unsigned int range);

/* Decode a range without modelling                          */
  U64 readRange64(U64 range);

/* Decode an unsigned char without modelling                 */
  unsigned char readByte();

/* Decode an unsigned short without modelling                */
  unsigned short readShort();

/* Decode an unsigned int without modelling                  */
  unsigned int readInt();

/* Decode a float without modelling (endian-ness dependent)  */
  float readFloat();

/* Decode an unsigned int64 without modelling                */
  U64 readInt64();

/* Decode a double without modelling                         */
  double readDouble();

/* Finish decoding                                           */
  void done();

private:
/* Calculate culmulative frequency for next symbol. Does NO update!*/
/* tot_f is the total frequency                              */
/* or: totf is 1<<shift                                      */
/* returns the <= culmulative frequency                      */
  unsigned int culshift( unsigned int shift );

/* Update decoding state                                     */
/* sy_f is the interval length (frequency of the symbol)     */
/* lt_f is the lower end (frequency sum of < symbols)        */
/* tot_f is the total interval length (total frequency sum)  */
  void update( unsigned int sy_f, unsigned int lt_f, unsigned int tot_f);

  inline void normalize();
  inline unsigned int inbyte();

  FILE* fp;

  unsigned char* chars;
  int current_char;
  int number_chars;

  unsigned int low;         /* low end of interval */
  unsigned int range;       /* length of interval */
  unsigned int help;        /* bytes_to_follow resp. intermediate value */
  unsigned char buffer;      /* buffer for output */
};

#endif
