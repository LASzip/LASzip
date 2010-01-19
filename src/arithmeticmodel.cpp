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

  FILE:  arithmeticmodel.cpp
  
  CONTENTS:
      
    see header file

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2009  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see header file

===============================================================================
*/
#include "arithmeticmodel.h"

#include <stdio.h>
#include <stdlib.h>

void AC_ERROR(const char* msg) {
  fprintf(stderr, "Arithmetic coding ERROR: %s\n", msg); exit(1);
}

ArithmeticModel::ArithmeticModel(unsigned int n, unsigned int *init, int compress)
{
  data_symbols = 0;
  distribution = 0;
  reset(n, init, compress);
}

ArithmeticModel::~ArithmeticModel()
{
  if (distribution) delete [] distribution;
}

void ArithmeticModel::reset(unsigned int n, unsigned int *init, int compress)
{
  if ( (n < 2) || (n > (1 << 11)) ) AC_ERROR("invalid number of symbols");

  if (data_symbols != n) { 
    data_symbols = n;
    last_symbol = data_symbols - 1;
    if (distribution) delete [] distribution;
                                     // define size of table for fast decoding
    if ((compress == 0) && (data_symbols > 16)) {
      unsigned int table_bits = 3;
      while (data_symbols > (1U << (table_bits + 2))) ++table_bits;
      table_size  = 1 << table_bits;
      table_shift = DM__LengthShift - table_bits;
      distribution = new unsigned int[2*data_symbols+table_size+2];
      decoder_table = distribution + 2 * data_symbols;
    }
    else {                                  // small alphabet: no table needed
      decoder_table = 0;
      table_size = table_shift = 0;
      distribution = new unsigned int[2*data_symbols];
    }
    if (distribution == 0) AC_ERROR("cannot allocate model memory");
    symbol_count = distribution + data_symbols;
  }

  total_count = 0;
  update_cycle = data_symbols;
  if (init)
    for (unsigned int k = 0; k < data_symbols; k++) symbol_count[k] = init[k];
  else
    for (unsigned int k = 0; k < data_symbols; k++) symbol_count[k] = 1;

  update(compress == 1);
  symbols_until_update = update_cycle = (data_symbols + 6) >> 1;
}

void ArithmeticModel::update(bool from_encoder)
{
                                   // halve counts when a threshold is reached
  if ((total_count += update_cycle) > DM__MaxCount) {
    total_count = 0;
    for (unsigned int n = 0; n < data_symbols; n++)
      total_count += (symbol_count[n] = (symbol_count[n] + 1) >> 1);
  }
                             // compute cumulative distribution, decoder table
  unsigned int k, sum = 0, s = 0;
  unsigned int scale = 0x80000000U / total_count;

  if (from_encoder || (table_size == 0))
    for (k = 0; k < data_symbols; k++) {
      distribution[k] = (scale * sum) >> (31 - DM__LengthShift);
      sum += symbol_count[k];
    }
  else {
    for (k = 0; k < data_symbols; k++) {
      distribution[k] = (scale * sum) >> (31 - DM__LengthShift);
      sum += symbol_count[k];
      unsigned int w = distribution[k] >> table_shift;
      while (s < w) decoder_table[++s] = k - 1;
    }
    decoder_table[0] = 0;
    while (s <= table_size) decoder_table[++s] = data_symbols - 1;
  }
                                             // set frequency of model updates
  update_cycle = (5 * update_cycle) >> 2;
  unsigned int max_cycle = (data_symbols + 6) << 3;
  if (update_cycle > max_cycle) update_cycle = max_cycle;
  symbols_until_update = update_cycle;
}

ArithmeticBitModel::ArithmeticBitModel()
{
  reset();
}

ArithmeticBitModel::~ArithmeticBitModel()
{
}

void ArithmeticBitModel::reset()
{
                                       // initialization to equiprobable model
  bit_0_count = 1;
  bit_count   = 2;
  bit_0_prob  = 1U << (BM__LengthShift - 1);
  update_cycle = bits_until_update = 4;         // start with frequent updates
}

void ArithmeticBitModel::update()
{
                                   // halve counts when a threshold is reached
  if ((bit_count += update_cycle) > BM__MaxCount) {
    bit_count = (bit_count + 1) >> 1;
    bit_0_count = (bit_0_count + 1) >> 1;
    if (bit_0_count == bit_count) ++bit_count;
  }
                                           // compute scaled bit 0 probability
  unsigned int scale = 0x80000000U / bit_count;
  bit_0_prob = (bit_0_count * scale) >> (31 - BM__LengthShift);

                                             // set frequency of model updates
  update_cycle = (5 * update_cycle) >> 2;
  if (update_cycle > 64) update_cycle = 64;
  bits_until_update = update_cycle;
}
