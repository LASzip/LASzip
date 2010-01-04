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

  FILE:  arithmeticdecoder.h
  
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
#ifndef ARITHMETIC_DECODER_H
#define ARITHMETIC_DECODER_H



#include "mydefs.h"
#include <stdio.h>

#include "arithmeticmodel.h"

namespace laszipalternate {

class ArithmeticDecoder
{
public:

/* Start the decoder                                         */
  ArithmeticDecoder(unsigned char* chars, unsigned int number_chars);
  ArithmeticDecoder(FILE* fp);
  ~ArithmeticDecoder();

/* Decode a bit with modelling                               */
  unsigned int decode(ArithmeticBitModel* m);

/* Decode s symbol with modelling                            */
  unsigned int decode(ArithmeticModel* m);

/* Decode bits without modelling                             */
  unsigned int readBits(unsigned int bits);

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

  unsigned int get_inbyte();
  void renorm_dec_interval();

  FILE* fp;

  unsigned char* inbuffer;
  unsigned char* endbuffer;
  unsigned int base, value, length;
};

} //namespace
#endif
