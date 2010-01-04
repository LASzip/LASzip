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

  FILE:  laspointwriter0compressed.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/

#include "laspointwriter0compressed.h"

namespace laszipalternate {

bool LASpointWriter0compressed::write_point(LASpoint* point, double gps_time, unsigned short* rgb)
{
	if (enc)
	{
    // find the median difference among the three preceding differences
    int median_x;
    if (last_x_diff[last_dir][0] < last_x_diff[last_dir][1])
    {
      if (last_x_diff[last_dir][1] < last_x_diff[last_dir][2])
        median_x = last_x_diff[last_dir][1];
      else if (last_x_diff[last_dir][0] < last_x_diff[last_dir][2])
        median_x = last_x_diff[last_dir][2];
      else
        median_x = last_x_diff[last_dir][0];
    }
    else
    {
      if (last_x_diff[last_dir][0] < last_x_diff[last_dir][2])
        median_x = last_x_diff[last_dir][0];
      else if (last_x_diff[last_dir][1] < last_x_diff[last_dir][2])
        median_x = last_x_diff[last_dir][2];
      else
        median_x = last_x_diff[last_dir][1];
    }

    int median_y;
    if (last_y_diff[last_dir][0] < last_y_diff[last_dir][1])
    {
      if (last_y_diff[last_dir][1] < last_y_diff[last_dir][2])
        median_y = last_y_diff[last_dir][1];
      else if (last_y_diff[last_dir][0] < last_y_diff[last_dir][2])
        median_y = last_y_diff[last_dir][2];
      else
        median_y = last_y_diff[last_dir][0];
    }
    else
    {
      if (last_y_diff[last_dir][0] < last_y_diff[last_dir][2])
        median_y = last_y_diff[last_dir][0];
      else if (last_y_diff[last_dir][1] < last_y_diff[last_dir][2])
        median_y = last_y_diff[last_dir][2];
      else
        median_y = last_y_diff[last_dir][1];
    }

		// compress x y z coordinates
		int x_diff = point->x - last_point.x;
		int y_diff = point->y - last_point.y;

    // if there is a direction flag we use it to encode the x and y differences with different contexts
  	ic_dx->CompressLast(median_x, x_diff, last_dir);

    // we use the difference between the x diff prediction and the acual value to switch contexts
    float messy_measure = (float)x_diff/(float)median_x;
    int messy = (int)((messy_measure-0.5f)/0.2f);

    if (messy < 0 || messy > 6)
    {
      ic_dy->CompressNone(y_diff);
      ic_z->CompressNone(point->z);
    }
    else
    {
      ic_dy->CompressLast(median_y, y_diff, (last_dir?messy+7:messy));
		  ic_z->CompressLast(last_point.z, point->z, messy);
    }

		// compress which other values have changed
		int changed_values = ((last_point.intensity != point->intensity) << 5) |
												 ((((unsigned char*)&last_point)[14] != ((unsigned char*)point)[14]) << 4) |
												 ((last_point.classification != point->classification) << 3) |
												 ((last_point.scan_angle_rank != point->scan_angle_rank) << 2) |
												 ((last_point.user_data != point->user_data) << 1) |
												 ((last_point.point_source_ID != point->point_source_ID));

		enc->encode(m_changed_values, changed_values);

		// compress the intensity if it has changed
		if (changed_values & 32)
		{
			ic_intensity->CompressLast(last_point.intensity, point->intensity);
		}

		// compress the edge_of_flight_line, scan_direction_flag, ... if it has changed
		if (changed_values & 16)
		{
			enc->encode(m_bit_byte, ((unsigned char*)point)[14]);
		}

		// compress the classification ... if it has changed
		if (changed_values & 8)
		{
			enc->encode(m_classification, point->classification);
		}
		
		// compress the scan_angle_rank ... if it has changed
		if (changed_values & 4)
		{
			ic_scan_angle_rank->CompressLast(last_point.scan_angle_rank, point->scan_angle_rank, messy == 2); // this is the best messy
		}

		// compress the user_data ... if it has changed
		if (changed_values & 2)
		{
			enc->encode(m_user_data, point->user_data);
		}

		// compress the point_source_ID ... if it has changed
		if (changed_values & 1)
		{
			ic_point_source_ID->CompressLast(last_point.point_source_ID, point->point_source_ID);
		}

    // only record the difference if the scan direction has not changed
    if (last_dir == point->scan_direction_flag)
    {
      last_x_diff[last_dir][last_incr[last_dir]] = x_diff;
      last_y_diff[last_dir][last_incr[last_dir]] = y_diff;

      last_incr[last_dir]++;
      if (last_incr[last_dir] > 2) last_incr[last_dir] = 0;
    }
  }
	else
	{
		fwrite(point, sizeof(LASpoint), 1, file);
		init_encoder();
	}

  last_dir = point->scan_direction_flag;
	last_point = *point;

	return true;
}

LASpointWriter0compressed::LASpointWriter0compressed(FILE* file)
{
	this->file = file;
  last_dir = 0;
	last_x_diff[0][0] = last_x_diff[0][1] = last_x_diff[0][2] = last_x_diff[1][0] = last_x_diff[1][1] = last_x_diff[1][2] = 0;
	last_y_diff[0][0] = last_y_diff[0][1] = last_y_diff[0][2] = last_y_diff[1][0] = last_y_diff[1][1] = last_y_diff[1][2] = 0;
  last_incr[0] = last_incr[1] = 0;
	enc = 0;
}

void LASpointWriter0compressed::init_encoder()
{
	enc = new ArithmeticEncoder(file,false);

	ic_dx = new IntegerCompressorContext();
	ic_dy = new IntegerCompressorContext();
	ic_z = new IntegerCompressorContext();

	ic_dx->SetPrecision(32); 
	ic_dy->SetPrecision(32); 
	ic_z->SetPrecision(32); 

  ic_dx->SetupCompressor(enc, 2);
  ic_dy->SetupCompressor(enc, 14);
  ic_z->SetupCompressor(enc, 7);

	m_changed_values = new ArithmeticModel(64,0,1);

	ic_intensity = new IntegerCompressorContext();
	ic_intensity->SetPrecision(16);
  ic_intensity->SetupCompressor(enc);

	m_bit_byte = new ArithmeticModel(256,0,1);
	m_classification = new ArithmeticModel(256,0,1);

	ic_scan_angle_rank = new IntegerCompressorContext();
	ic_scan_angle_rank->SetPrecision(8);
  ic_scan_angle_rank->SetupCompressor(enc, 2);

	m_user_data = new ArithmeticModel(256,0,1);

	ic_point_source_ID = new IntegerCompressorContext();
	ic_point_source_ID->SetPrecision(16);
  ic_point_source_ID->SetupCompressor(enc);
}

LASpointWriter0compressed::~LASpointWriter0compressed()
{
	if (enc)
	{
		enc->done();

		delete enc;
		delete ic_dx;
		delete ic_dy;
		delete ic_z;
		delete m_changed_values;
		delete ic_intensity;
		delete m_bit_byte;
		delete m_classification;
		delete ic_scan_angle_rank;
		delete m_user_data;
		delete ic_point_source_ID;
	}
}

}
