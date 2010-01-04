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
  
    31 October 2009 -- updated to use the new Arithmetic Coder 
    7 September 2008 -- updated to support LAS format 1.2 
    22 February 2007 -- created 45 minutes before henna's 32nd birthday
  
===============================================================================
*/
#ifndef LAS_POINT_READER_0COMPRESSED_H
#define LAS_POINT_READER_0COMPRESSED_H

#include "laspointreader.h"

#include "arithmeticmodel.h"
#include "arithmeticdecoder.h"
#include "integercompressor_newest.h"

#include <stdio.h>

namespace laszipalternate {
	
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
  ArithmeticDecoder* dec;
  IntegerCompressorContext* ic_dx;
  IntegerCompressorContext* ic_dy;
  IntegerCompressorContext* ic_z;
  ArithmeticModel* m_changed_values;
  IntegerCompressorContext* ic_intensity;
  ArithmeticModel* m_bit_byte;
  ArithmeticModel* m_classification;
  IntegerCompressorContext* ic_scan_angle_rank;
  ArithmeticModel* m_user_data;
  IntegerCompressorContext* ic_point_source_ID;
};

}
#endif
