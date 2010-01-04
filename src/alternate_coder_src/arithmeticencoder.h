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


#include "mydefs.h"
#include "arithmeticmodel.h"
#include <stdio.h>

namespace laszipalternate {

class ArithmeticEncoder
{
public:

/* Start the encoder                                         */
  ArithmeticEncoder(FILE* fp, int store_bytes = 0);
  ~ArithmeticEncoder();

/* Encode bit with modelling                                 */
  void encode(ArithmeticBitModel* m, unsigned int sym);

/* Encode symbol with modelling                              */
  void encode(ArithmeticModel* m, unsigned int sym);

/* Encode bits without modelling                             */
  void writeBits(unsigned int bits, unsigned int sym);

/* Encode an unsigned char without modelling                 */
  void writeByte(unsigned char b);

/* Encode an unsigned short without modelling                */
  void writeShort(unsigned short s);

/* Encode an unsigned int without modelling                  */
  void writeInt(unsigned int i);

/* Encode a float without modelling                          */
  void writeFloat(float f);

/* Encode an unsigned int64 without modelling                */
  void writeInt64(U64 l);

/* Encode a double without modelling                         */
  void writeDouble(double d);

/* Finish encoding, returns number of bytes written          */
  unsigned int done();

  unsigned char* getBytes();
  unsigned int getNumberBytes();

private:
  void propagate_carry();
  void renorm_enc_interval();
  void manage_outbuffer();

  FILE* fp;
  bool store_bytes;

  unsigned char* outbuffer;
  unsigned char* endbuffer;
  unsigned char* outbyte;
  unsigned char* endbyte;
  unsigned int bytes_output;
  unsigned int base, value, length;
};

}
#endif
