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

  FILE:  laspointwriter2compressed.h
  
  CONTENTS:
  
    Writes a point of type 2 (with rgb color) in our compressed LAS format 1.2

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
#ifndef LAS_POINT_WRITER_2COMPRESSED_H
#define LAS_POINT_WRITER_2COMPRESSED_H

#include "laspointwriter.hpp"

#include "rangemodel.hpp"
#include "rangeencoder.hpp"
#include "rangeintegercompressor.hpp"

#include <stdio.h>

class LASpointWriter2compressed : public LASpointWriter
{
public:
  bool write_point(LASpoint* point, double gps_time = 0, unsigned short* rgb = 0);
  LASpointWriter2compressed(FILE* file);
  ~LASpointWriter2compressed();

private:
  FILE* file;
  LASpoint last_point;
  int last_dir;
  int last_x_diff[2][3];
  int last_y_diff[2][3];
  int last_incr[2];
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
  RangeIntegerCompressor* ic_r;
  RangeIntegerCompressor* ic_g;
  RangeIntegerCompressor* ic_b;
};

#endif
