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

  FILE:  laspointreader0compressedarithmetic.h
  
  CONTENTS:
  
    Reads a point of type 0 (without gps_time) from our compressed LAS format 1.1

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    31 October 2009 -- updated to use the new Arithmetic Coder 
    7 September 2008 -- updated to support LAS format 1.2 
    22 February 2007 -- created 45 minutes before henna's 32nd birthday
  
===============================================================================
*/
#ifndef LAS_POINT_READER_0COMPRESSED_ARITHMETIC_H
#define LAS_POINT_READER_0COMPRESSED_ARITHMETIC_H

#include "laspointreader.h"

#include "arithmeticmodel.h"
#include "arithmeticdecoder.h"
#include "arithmeticintegercompressor.h"

#include <stdio.h>

class LASpointReader0compressedArithmetic : public LASpointReader
{
public:
  bool read_point(LASpoint* point, double* gps_time = 0, unsigned short* rgb = 0);
  LASpointReader0compressedArithmetic(FILE* file);
  ~LASpointReader0compressedArithmetic();

private:
  FILE* file;
  LASpoint last_point;
  int last_dir;
  int last_x_diff[2][3];
  int last_y_diff[2][3];
  int last_incr[2];
  void init_decoder();
  ArithmeticDecoder* dec;
  ArithmeticIntegerCompressor* ic_dx;
  ArithmeticIntegerCompressor* ic_dy;
  ArithmeticIntegerCompressor* ic_z;
  ArithmeticModel* m_changed_values;
  ArithmeticIntegerCompressor* ic_intensity;
  ArithmeticModel* m_bit_byte;
  ArithmeticModel* m_classification;
  ArithmeticIntegerCompressor* ic_scan_angle_rank;
  ArithmeticModel* m_user_data;
  ArithmeticIntegerCompressor* ic_point_source_ID;
};

#endif
