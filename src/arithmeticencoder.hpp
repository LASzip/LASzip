/******************************************************************************
 *
 * Project:  integrating laszip into liblas - http://liblas.org -
 * Purpose:
 * Author:   Martin Isenburg
 *           isenburg at cs.unc.edu
 *
 ******************************************************************************
 * Copyright (c) 2010, Martin Isenburg
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

  FILE:  arithmeticencoder.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2009  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    8 December 2010 -- unified framework for all entropy coders
    30 October 2009 -- merged michael schindler's and amir said's code
  
===============================================================================
*/
#ifndef ARITHMETIC_ENCODER_H
#define ARITHMETIC_ENCODER_H

#include "entropyencoder.hpp"

class ArithmeticEncoder : public EntropyEncoder
{
public:

/* Constructor & Destructor                                  */
  ArithmeticEncoder();
  ~ArithmeticEncoder();

/* Manage encoding                                           */
  I32 init(ByteStreamOut* outstream);
  void done();

/* Manage an entropy model for a single bit                  */
  EntropyModel* createBitModel();
  void initBitModel(EntropyModel* model);
  void destroyBitModel(EntropyModel* model);

/* Manage an entropy model for n symbols (table optional)    */
  EntropyModel* createSymbolModel(U32 n);
  void initSymbolModel(EntropyModel* model, U32 *table=0);
  void destroySymbolModel(EntropyModel* model);

/* Encode a bit with modelling                               */
  void encodeBit(EntropyModel* model, U32 sym);

/* Encode a symbol with modelling                            */
  void encodeSymbol(EntropyModel* model, U32 sym);

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

private:

  ByteStreamOut* outstream;

  void propagate_carry();
  void renorm_enc_interval();
  void manage_outbuffer();
  U8* outbuffer;
  U8* endbuffer;
  U8* outbyte;
  U8* endbyte;
  U32 base, value, length;
};

#endif
