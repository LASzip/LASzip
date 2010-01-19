/*
===============================================================================

  FILE:  laspointreader3compressedarithmetic.h
  
  CONTENTS:
  
    Reads a point of type 3 (with gps_time and rgb color) from our compressed LAS format 1.2

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    31 October 2009 -- updated to use the new Arithmetic Coder 
    7 September 2008 -- created after baking three apple pies with Kaoru 
  
===============================================================================
*/
#ifndef LAS_POINT_READER_3COMPRESSED_ARITHMETIC_H
#define LAS_POINT_READER_3COMPRESSED_ARITHMETIC_H

#include "laspointreader.h"

#include "arithmeticmodel.h"
#include "arithmeticdecoder.h"
#include "arithmeticintegercompressor.h"

#include <stdio.h>

class LASpointReader3compressedArithmetic : public LASpointReader
{
public:
  bool read_point(LASpoint* point, double* gps_time = 0, unsigned short* rgb = 0);
  LASpointReader3compressedArithmetic(FILE* file);
  ~LASpointReader3compressedArithmetic();

private:
  FILE* file;
  LASpoint last_point;
  int last_dir;
  int last_x_diff[2][3];
  int last_y_diff[2][3];
  int last_incr[2];
  I64F64 last_gps_time;
  I32 last_gps_time_diff;
  unsigned short last_rgb[3];
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
  ArithmeticIntegerCompressor* ic_gps_time;
  ArithmeticModel** m_gps_time_multi;
  int multi_extreme_counter;
  ArithmeticIntegerCompressor* ic_r;
  ArithmeticIntegerCompressor* ic_g;
  ArithmeticIntegerCompressor* ic_b;
};

#endif
