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

  FILE:  arithmeticencoder.h
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2009  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    30 October 2009 -- merged michael schindler's and amir said's code
  
===============================================================================
*/
#ifndef ARITHMETIC_ENCODER_H
#define ARITHMETIC_ENCODER_H

#include "arithmeticmodel.h"

class ArithmeticEncoder
{
public:

/* Start the encoder                                         */
  ArithmeticEncoder(FILE* fp, bool store_bytes = false);
  ~ArithmeticEncoder();

/* Encode bit with modelling                                 */
  void encode(ArithmeticBitModel* m, U32 sym);

/* Encode symbol with modelling                              */
  void encode(ArithmeticModel* m, U32 sym);

/* Encode a bit without modelling                            */
  void writeBit(U32 sym);

/* Encode bits without modelling                             */
  void writeBits(U32 bits, U32 sym);

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

  U8* getBytes();
  U32 getNumberBytes();

private:
  void propagate_carry();
  void renorm_enc_interval();
  void manage_outbuffer();

  FILE* fp;
  bool store_bytes;

  U8* outbuffer;
  U8* endbuffer;
  U8* outbyte;
  U8* endbyte;
  U32 bytes_output;
  U32 base, value, length;
};

#endif
