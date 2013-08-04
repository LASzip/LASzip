/*
===============================================================================

  FILE:  entropydecoder.hpp
  
  CONTENTS:
      
    Abstract base class for all entropy decoders.

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2005-2012, martin isenburg, rapidlasso - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration  
    8 December 2010 -- unified framework for all entropy coders
  
===============================================================================
*/
#ifndef ENTROPY_DECODER_H
#define ENTROPY_DECODER_H

#include "mydefs.hpp"
#include "bytestreamin.hpp"

class EntropyModel;

class EntropyDecoder
{
public:

/* Destructor                                                */
  virtual ~EntropyDecoder() {};

/* Manage decoding                                           */
  virtual BOOL init(ByteStreamIn* instream) = 0;
  virtual void done() = 0;

/* Manage an entropy model for a single bit                  */
  virtual EntropyModel* createBitModel() = 0;
  virtual void initBitModel(EntropyModel* model) = 0;
  virtual void destroyBitModel(EntropyModel* model) = 0;

/* Manage an entropy model for n symbols (table optional)    */
  virtual EntropyModel* createSymbolModel(U32 n) = 0;
  virtual void initSymbolModel(EntropyModel* model, U32 *init=0) = 0;
  virtual void destroySymbolModel(EntropyModel* model) = 0;

/* Decode a bit with modelling                               */
  virtual U32 decodeBit(EntropyModel* model) = 0;

/* Decode a symbol with modelling                            */
  virtual U32 decodeSymbol(EntropyModel* model) = 0;

/* Decode a bit without modelling                            */
  virtual U32 readBit() = 0;

/* Decode bits without modelling                             */
  virtual U32 readBits(U32 bits) = 0;

/* Decode an unsigned char without modelling                 */
  virtual U8 readByte() = 0;

/* Decode an unsigned short without modelling                */
  virtual U16 readShort() = 0;

/* Decode an unsigned int without modelling                  */
  virtual U32 readInt() = 0;

/* Decode a float without modelling                          */
  virtual F32 readFloat() = 0;

/* Decode an unsigned 64 bit int without modelling           */
  virtual U64 readInt64() = 0;

/* Decode a double without modelling                         */
  virtual F64 readDouble() = 0;
};

#endif
