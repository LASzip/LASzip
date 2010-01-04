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

  FILE:  laspointwriter0compressed.h
  
  CONTENTS:
  
    Writes a point of type 0 (without gps_time) in our compressed LAS format 1.1

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
    21 February 2007 -- created after having the longest talk ever with my aunt
  
===============================================================================
*/
#ifndef LAS_POINT_WRITER_0COMPRESSED_H
#define LAS_POINT_WRITER_0COMPRESSED_H

#include "laspointwriter.h"

#include "arithmeticmodel.h"
#include "arithmeticencoder.h"
#include "integercompressor_newest.h"

#include <stdio.h>

namespace laszipalternate {


class LASpointWriter0compressed : public LASpointWriter
{
public:
  bool write_point(LASpoint* point, double gps_time = 0, unsigned short* rgb = 0);
  LASpointWriter0compressed(FILE* file);
  ~LASpointWriter0compressed();

private:
  FILE* file;
  LASpoint last_point;
  int last_dir;
  int last_x_diff[2][3];
  int last_y_diff[2][3];
  int last_incr[2];
  void init_encoder();
  ArithmeticEncoder* enc;
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
