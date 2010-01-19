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

  FILE:  arithmeticmodel.h
  
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
#ifndef ARITHMETIC_MODEL_H
#define ARITHMETIC_MODEL_H

#include <stdio.h>
#include <stdlib.h>

#include "mydefs.h"

/* this header byte needs to change in case incompatible change happen */
#define AC_HEADER_BYTE 2
#define AC_BUFFER_SIZE 1024

void AC_ERROR(const char* msg);

const U32 AC__MinLength = 0x01000000U;   // threshold for renormalization
const U32 AC__MaxLength = 0xFFFFFFFFU;      // maximum AC interval length

                                           // Maximum values for binary models
const U32 BM__LengthShift = 13;     // length bits discarded before mult.
const U32 BM__MaxCount    = 1 << BM__LengthShift;  // for adaptive models

                                          // Maximum values for general models
const U32 DM__LengthShift = 15;     // length bits discarded before mult.
const U32 DM__MaxCount    = 1 << DM__LengthShift;  // for adaptive models

class ArithmeticModel
{
public:
  ArithmeticModel(U32 n, U32 *init, int compress);
  ~ArithmeticModel();

  void reset(U32 n, U32 *init, int compress);

private:
  void update(bool);
  U32 * distribution, * symbol_count, * decoder_table;
  U32 total_count, update_cycle, symbols_until_update;
  U32 data_symbols, last_symbol, table_size, table_shift;
  friend class ArithmeticEncoder;
  friend class ArithmeticDecoder;
};

class ArithmeticBitModel
{
public:
  ArithmeticBitModel();
  ~ArithmeticBitModel();

  void reset();

private:
  void update();
  U32 update_cycle, bits_until_update;
  U32 bit_0_prob, bit_0_count, bit_count;
  friend class ArithmeticEncoder;
  friend class ArithmeticDecoder;
};

#endif
