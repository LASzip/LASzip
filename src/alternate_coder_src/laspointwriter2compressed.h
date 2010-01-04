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
  
    31 October 2009 -- updated to use the new Arithmetic Coder 
    7 September 2008 -- created after baking three apple pies with Kaoru 
  
===============================================================================
*/
#ifndef LAS_POINT_WRITER_2COMPRESSED_H
#define LAS_POINT_WRITER_2COMPRESSED_H

#include "laspointwriter.h"

#include "Arithmeticmodel.h"
#include "Arithmeticencoder.h"
#include "integercompressor_newest.h"

#include <stdio.h>

namespace laszipalternate {

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
  IntegerCompressorContext* ic_r;
  IntegerCompressorContext* ic_g;
  IntegerCompressorContext* ic_b;
};

}
#endif
