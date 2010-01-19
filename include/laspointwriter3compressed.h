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

  FILE:  laspointwriter3compressed.h
  
  CONTENTS:
  
    Writes a point of type 3 (with gps_time and rgb color) in our compressed LAS format 1.2

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- created after baking three apple pies with Kaoru 
  
===============================================================================
*/
#ifndef LAS_POINT_WRITER_3COMPRESSED_H
#define LAS_POINT_WRITER_3COMPRESSED_H

#include "laspointwriter.h"

#include "rangemodel.h"
#include "rangeencoder.h"
#include "rangeintegercompressor.h"

#include <stdio.h>

class LASpointWriter3compressed : public LASpointWriter
{
public:
  bool write_point(LASpoint* point, double gps_time = 0, unsigned short* rgb = 0);
  LASpointWriter3compressed(FILE* file);
  ~LASpointWriter3compressed();

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
  void init_encoder();
  RangeEncoder* re;
  RangeIntegerCompressor* ic_dx;
  RangeIntegerCompressor* ic_dy;
  RangeIntegerCompressor* ic_z;
  RangeModel* rm_changed_values;
  RangeIntegerCompressor* ic_intensity;
  RangeModel* rm_bit_byte;
  RangeModel* rm_classification;
  RangeIntegerCompressor* ic_scan_angle_rank;
  RangeModel* rm_user_data;
  RangeIntegerCompressor* ic_point_source_ID;
  RangeIntegerCompressor* ic_gps_time;
  RangeModel** rm_gps_time_multi;
  int multi_extreme_counter;
  RangeIntegerCompressor* ic_r;
  RangeIntegerCompressor* ic_g;
  RangeIntegerCompressor* ic_b;
};

#endif
