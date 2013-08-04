/*
===============================================================================

  FILE:  arithmeticdecoder.hpp
  
  CONTENTS:
      
    A modular C++ wrapper for an adapted version of Amir Said's FastAC Code.
    see: http://www.cipr.rpi.edu/~said/FastAC.html

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2007-2012, martin isenburg, rapidlasso - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
    8 December 2010 -- unified framework for all entropy coders
    30 October 2009 -- refactoring Amir Said's FastAC code
  
===============================================================================
*/
#ifndef ARITHMETIC_DECODER_H
#define ARITHMETIC_DECODER_H

#include "entropydecoder.hpp"

class ArithmeticDecoder : public EntropyDecoder
{
public:

/* Constructor & Destructor                                  */
  ArithmeticDecoder();
  ~ArithmeticDecoder();

/* Manage decoding                                           */
  BOOL init(ByteStreamIn* instream);
  void done();

/* Manage an entropy model for a single bit                  */
  EntropyModel* createBitModel();
  void initBitModel(EntropyModel* model);
  void destroyBitModel(EntropyModel* model);

/* Manage an entropy model for n symbols (table optional)    */
  EntropyModel* createSymbolModel(U32 n);
  void initSymbolModel(EntropyModel* model, U32* table=0);
  void destroySymbolModel(EntropyModel* model);

/* Decode a bit with modelling                               */
  U32 decodeBit(EntropyModel* model);

/* Decode a symbol with modelling                            */
  U32 decodeSymbol(EntropyModel* model);

/* Decode a bit without modelling                            */
  U32 readBit();

/* Decode bits without modelling                             */
  U32 readBits(U32 bits);

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
  F64 readDouble();

private:

  ByteStreamIn* instream;

  void renorm_dec_interval();
  U32 base, value, length;
};

#endif
