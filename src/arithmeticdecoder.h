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

#include "arithmeticmodel.h"

class ArithmeticDecoder
{
public:

/* Start the decoder                                         */
  ArithmeticDecoder(U8* chars, U32 number_chars);
  ArithmeticDecoder(FILE* fp);
  ~ArithmeticDecoder();

/* Decode a bit with modelling                               */
  U32 decode(ArithmeticBitModel* m);

/* Decode s symbol with modelling                            */
  U32 decode(ArithmeticModel* m);

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

/* Decode a float without modelling (endian-ness dependent)  */
  F32 readFloat();

/* Decode an unsigned 64 bit int without modelling           */
  U64 readInt64();

/* Decode a double without modelling                         */
  F64 readDouble();

/* Finish decoding                                           */
  void done();

private:

  U32 get_inbyte();
  void renorm_dec_interval();

  FILE* fp;

  U8* inbuffer;
  U8* endbuffer;
  U32 base, value, length;
};

#endif
