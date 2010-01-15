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

class RangeEncoder
{
public:

/* Start the encoder                                         */
  RangeEncoder(FILE* fp, bool store_chars = true);
  ~RangeEncoder();

/* Encode with modelling                                     */
  void encode(RangeModel* rm, U32 sym);

/* Encode bits without modelling                             */
  void writeBits(U32 bits, U32 sym);

/* Encode a range without modelling                          */
  void writeRange(U32 range, U32 sym);

/* Encode a range without modelling                          */
  void writeRange64(U64 range, U64 sym);

/* Encode an unsigned char without modelling                 */
  void writeByte(U8 sym);

/* Encode an unsigned short without modelling                */
  void writeShort(U16 sym);

/* Encode an unsigned int without modelling                  */
  void writeInt(U32 sym);

/* Encode a float without modelling                          */
  void writeFloat(F32 sym);

/* Encode an unsigned 64 bit int without modelling           */
  void writeInt64(U64 sym);

/* Encode a double without modelling                         */
  void writeDouble(F64 sym);

/* Finish encoding, returns number of bytes written          */
  U32 done();

  U8* getChars();
  U32 getNumberChars();

  U32 getNumberBytes();

private:
  inline void normalize();
  inline void outbyte(U32 byte);

  FILE* fp;

  U8* chars;
  U32 number_chars;
  U32 allocated_chars;

  U32 low;           /* low end of interval */
  U32 range;         /* length of interval */
  U32 help;          /* bytes_to_follow resp. intermediate value */
  U8 buffer;         /* buffer for input/output */
  /* the following is used only when encoding */
  U32 bytecount;     /* counter for output bytes  */
};

#endif
