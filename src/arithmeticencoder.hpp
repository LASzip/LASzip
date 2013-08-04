/*
===============================================================================

  FILE:  arithmeticencoder.hpp
  
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
  BOOL init(ByteStreamOut* outstream);
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
