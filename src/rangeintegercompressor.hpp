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

  FILE:  rangeintegercompressor.h
  
  CONTENTS:
 
    This compressor provides three different contexts for encoding integer
    numbers whose range may lie anywhere between 1 and 31 bits, which is
    specified with the SetPrecision function. Two of the encoding functions
    take a integer prediction as input. The other will predict the integer
    simply using the last integer that was encoded.

    The compressor encodes two things:

      (1) the number k of miss-predicted low-order bits and
      (2) the k-bit number that corrects the missprediction

    The k-bit number is usually coded broken in two chunks. The highest
    bits are compressed using an arithmetic range table. The lower bits
    are stored raw without predicive coding. How many of the higher bits
    are compressed can be specified with BITS_HIGH. The default is 8.

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2005  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    30 September 2005 -- now splitting the corrector into raw and compressed bits
    13 July 2005 -- created after returning with many mosquito bites from OBX
  
===============================================================================
*/
#ifndef RANGE_INTEGER_COMPRESSOR_H
#define RANGE_INTEGER_COMPRESSOR_H

#include "rangeencoder.hpp"
#include "rangedecoder.hpp"

class RangeIntegerCompressor
{
public:

  // SetPrecision:
  void SetPrecision(U32 bits) {this->bits = bits;};
  // GetPrecision:
  U32 GetPrecision() const {return bits;};

  // SetRange:
  void SetRange(U32 range) {this->range = range;};
  // GetRange:
  U32 GetRange() const {return range;};

  // SetupCompressor:
  void SetupCompressor(RangeEncoder* re, U32 contexts=1, U32 BITS_HIGH=8);
  void FinishCompressor();

  // Compress:
  void Compress(I32 iPred, I32 iReal, U32 context=0);

  // SetupDecompressor:
  void SetupDecompressor(RangeDecoder* rd, U32 contexts=1, U32 BITS_HIGH=8);
  void FinishDecompressor();

  // Decompress:
  I32 Decompress(I32 iPred, U32 context=0);

  // Get the k corrector bits from the last compress/decompress call
  U32 GetK() const {return k;};

  // Constructor:
  RangeIntegerCompressor();
  // Destructor:
  ~RangeIntegerCompressor();

private:
  // Private Functions
  void writeCorrector(I32 c, RangeModel* rmBits);
  I32 readCorrector(RangeModel* amBits);

  U32 k;

  U32 contexts;
  U32 bits_high;

  // Private Variables
  U32 bits;
  U32 range;

  U32 corr_bits;
  U32 corr_range;
  I32 corr_min;
  I32 corr_max;

  RangeEncoder* re;
  RangeDecoder* rd;

  RangeModel** rmBits;

  RangeModel** rmCorrector;

  int** corr_histogram;
};

#endif
