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

  FILE:  rangeencoder.h
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    Copyright (C) 2003 Martin Isenburg (isenburg@cs.unc.edu)
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    28 June 2004 -- added an option for NOT storing the code characters at all 
    14 January 2003 -- adapted from michael schindler's code before SIGGRAPH
  
===============================================================================
*/
#ifndef RANGEENCODER_H
#define RANGEENCODER_H

#include <stdio.h>

#include "rangemodel.h"
#include "mydefs.h"

class RangeEncoder
{
public:

/* Start the encoder                                         */
  RangeEncoder(FILE* fp, bool store_chars = true);
  ~RangeEncoder();

/* Encode with modelling                                     */
  void encode(RangeModel* rm, unsigned int sym);

/* Encode bits without modelling                            */
  void writeBits(unsigned int bits, unsigned int sym);

/* Encode a range without modelling                          */
  void writeRange(unsigned int range, unsigned int sym);

/* Encode a range without modelling                          */
  void writeRange64( U64 range,  U64 sym);

/* Encode an unsigned char without modelling                 */
  void writeByte(unsigned char b);

/* Encode an unsigned short without modelling                */
  void writeShort(unsigned short s);

/* Encode an unsigned int without modelling                  */
  void writeInt(unsigned int i);

/* Encode a float without modelling                          */
  void writeFloat(float f);

/* Encode an unsigned int64 without modelling                */
  void writeInt64( U64 l);

/* Encode a double without modelling                         */
  void writeDouble(double d);

/* Finish encoding, returns number of bytes written          */
  unsigned int done();

  unsigned char* getChars();
  int getNumberChars();

  long getNumberBits();
  int getNumberBytes();

private:
  inline void normalize();
  inline void outbyte(unsigned int byte);

  FILE* fp;

  unsigned char* chars;
  int number_chars;
  int allocated_chars;

  unsigned int low;           /* low end of interval */
  unsigned int range;         /* length of interval */
  unsigned int help;          /* bytes_to_follow resp. intermediate value */
  unsigned char buffer;       /* buffer for input/output */
  /* the following is used only when encoding */
  unsigned int bytecount;     /* counter for outputed bytes  */
};

#endif
