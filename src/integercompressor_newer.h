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

  FILE:  integercompressor_newer.h
  
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
#ifndef INTEGER_COMPRESSOR_NEWER_H
#define INTEGER_COMPRESSOR_NEWER_H

#include "mydefs.h"

#include "rangeencoder.h"
#include "rangedecoder.h"
#include "rangemodel.h"

class IntegerCompressorContext
{
public:

  // SetPrecision:
  void SetPrecision(I32 iBits);
  // GetPrecision:
  I32 GetPrecision();

  // SetRange:
  void SetRange(I32 iRange);
  // GetRange:
  I32 GetRange();

  // SetupCompressor:
  void SetupCompressor(RangeEncoder* re, int contexts=1, int BITS_HIGH=8);
  void FinishCompressor();

  // Compress:
  I32 CompressNone(I32 iReal);
  I32 CompressLast(I32 iPred, I32 iReal, int context=0);

  // SetupDecompressor:
  void SetupDecompressor(RangeDecoder* rd, int contexts=1, int BITS_HIGH=8);
  void FinishDecompressor();

  // Deompress:
  I32 DecompressNone();
  I32 DecompressLast(I32 iPred, int context=0);

  // Constructor:
  IntegerCompressorContext();
  // Destructor:
  ~IntegerCompressorContext();

private:
  // Private Functions
  I32 writeCorrector(I32 c, RangeModel* rmBits);
  I32 readCorrector(RangeModel* amBits);

  int contexts;
  int bits_high;

  // Private Variables
  int bits;
  int range;

  int corr_bits;
  int corr_range;
  int corr_max;
  int corr_min;

  int last;

  RangeEncoder* re;
  RangeDecoder* rd;

  RangeModel* rmBitsNone;
  RangeModel** rmBitsLast;

  RangeModel** rmCorrector;

  int** corr_histogram;
};

#endif
