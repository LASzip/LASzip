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

  FILE:  laspointreader0compressed.h
  
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
  
    7 September 2008 -- updated to support LAS format 1.2 
    22 February 2007 -- created 45 minutes before henna's 32nd birthday
  
===============================================================================
*/
#ifndef LAS_POINT_READER_0COMPRESSED_H
#define LAS_POINT_READER_0COMPRESSED_H

#include "laspointreader.h"

#include "rangemodel.h"
#include "rangedecoder.h"
#include "rangeintegercompressor.h"

#include <stdio.h>

class LASpointReader0compressed : public LASpointReader
{
public:
  bool read_point(LASpoint* point, double* gps_time = 0, unsigned short* rgb = 0);
  LASpointReader0compressed(FILE* file);
  ~LASpointReader0compressed();

private:
  FILE* file;
  LASpoint last_point;
  int last_dir;
  int last_x_diff[2][3];
  int last_y_diff[2][3];
  int last_incr[2];
  void init_decoder();
  RangeDecoder* rd;
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
};

#endif
