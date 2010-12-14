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

  FILE:  entropyencoder.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    8 December 2010 -- unified framework for all entropy coders
  
===============================================================================
*/
#ifndef ENTROPY_ENCODER_H
#define ENTROPY_ENCODER_H

#include "mydefs.hpp"
#include "bytestreamout.hpp"

class EntropyModel;

class EntropyEncoder
{
public:

/* Destructor                                                */
  virtual ~EntropyEncoder() {};

/* Manage decoding                                           */
  virtual I32 init(ByteStreamOut* outstream) = 0;
  virtual void done() = 0;

/* Manage an entropy model for a single bit                  */
  virtual EntropyModel* createBitModel() = 0;
  virtual void initBitModel(EntropyModel* model) = 0;
  virtual void destroyBitModel(EntropyModel* model) = 0;

/* Manage an entropy model for n symbols (table optional)    */
  virtual EntropyModel* createSymbolModel(U32 n) = 0;
  virtual void initSymbolModel(EntropyModel* model, U32 *init=0) = 0;
  virtual void destroySymbolModel(EntropyModel* model) = 0;

/* Encode a bit with modelling                               */
  virtual void encodeBit(EntropyModel* model, U32 bit)=0;

/* Encode a symbol with modelling                            */
  virtual void encodeSymbol(EntropyModel* model, U32 sym)=0;

/* Encode a bit without modelling                            */
  virtual void writeBit(U32 sym)=0;

/* Encode bits without modelling                             */
  virtual void writeBits(U32 bits, U32 sym)=0;

/* Encode an unsigned char without modelling                 */
  virtual void writeByte(U8 sym)=0;

/* Encode an unsigned short without modelling                */
  virtual void writeShort(U16 sym)=0;

/* Encode an unsigned int without modelling                  */
  virtual void writeInt(U32 sym)=0;

/* Encode a float without modelling                          */
  virtual void writeFloat(F32 sym)=0;

/* Encode an unsigned 64 bit int without modelling           */
  virtual void writeInt64(U64 sym)=0;

/* Encode a double without modelling                         */
  virtual void writeDouble(F64 sym)=0;
};

#endif
