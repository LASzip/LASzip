/******************************************************************************
 *
 * Project:  integrating laszip into liblas - http://liblas.org -
 * Purpose:
 * Author:   Martin Isenburg
 *           isenburg at cs.unc.edu
 *
 ******************************************************************************
 * Copyright (c) 2010, Martin Isenburg
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

  FILE:  laswriteitemcompressed_v1.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/

#include "laswriteitemcompressed_v1.hpp"

#include <assert.h>
#include <string.h>

/*
===============================================================================
                       LASwriteItemCompressed_POINT10_v1
===============================================================================
*/

struct LASpoint10
{
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned char return_number : 3;
  unsigned char number_of_returns_of_given_pulse : 3;
  unsigned char scan_direction_flag : 1;
  unsigned char edge_of_flight_line : 1;
  unsigned char classification;
  char scan_angle_rank;
  unsigned char user_data;
  unsigned short point_source_ID;
};

LASwriteItemCompressed_POINT10_v1::LASwriteItemCompressed_POINT10_v1(EntropyEncoder* enc)
{
  /* set encoder */
  assert(enc);
  this->enc = enc;

  /* create models and integer compressors */
  ic_dx = new IntegerCompressor(enc, 32, 2);  // 32 bits, 2 contexts
	ic_dy = new IntegerCompressor(enc, 32, 33); // 32 bits, 33 contexts
	ic_z = new IntegerCompressor(enc, 32, 33);  // 32 bits, 33 contexts
	m_changed_values = enc->createSymbolModel(64);
	ic_intensity = new IntegerCompressor(enc, 16);
	m_bit_byte = enc->createSymbolModel(256);
  m_classification = enc->createSymbolModel(256);
	ic_scan_angle_rank = new IntegerCompressor(enc, 8, 2);
	m_user_data = enc->createSymbolModel(256);
	ic_point_source_ID = new IntegerCompressor(enc, 16);

  /* create last item */
  last_item = new U8[20];
}

LASwriteItemCompressed_POINT10_v1::~LASwriteItemCompressed_POINT10_v1()
{
  delete ic_dx;
  delete ic_dy;
  delete ic_z;
  enc->destroySymbolModel(m_changed_values);
  delete ic_intensity;
  enc->destroySymbolModel(m_bit_byte);
  enc->destroySymbolModel(m_classification);
  delete ic_scan_angle_rank;
  enc->destroySymbolModel(m_user_data);
  delete ic_point_source_ID;
  delete [] last_item;
}

BOOL LASwriteItemCompressed_POINT10_v1::init(U8* item)
{
  /* init state */
  last_dir = 0;
	last_x_diff[0][0] = last_x_diff[0][1] = last_x_diff[0][2] = last_x_diff[1][0] = last_x_diff[1][1] = last_x_diff[1][2] = 0;
	last_y_diff[0][0] = last_y_diff[0][1] = last_y_diff[0][2] = last_y_diff[1][0] = last_y_diff[1][1] = last_y_diff[1][2] = 0;
  last_incr[0] = last_incr[1] = 0;

  /* init models and integer compressors */
  ic_dx->initCompressor();
  ic_dy->initCompressor();
  ic_z->initCompressor();
  enc->initSymbolModel(m_changed_values);
  ic_intensity->initCompressor();
  enc->initSymbolModel(m_bit_byte);
  enc->initSymbolModel(m_classification);
  ic_scan_angle_rank->initCompressor();
  enc->initSymbolModel(m_user_data);
  ic_point_source_ID->initCompressor();

  /* init last item */
  memcpy(last_item, item, 20);

  return TRUE;
}

BOOL LASwriteItemCompressed_POINT10_v1::write(U8* item)
{
  // find median difference for x and y from 3 preceding differences
  I32 median_x;
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

  I32 median_y;
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
  I32 x_diff = ((LASpoint10*)item)->x - ((LASpoint10*)last_item)->x;
  I32 y_diff = ((LASpoint10*)item)->y - ((LASpoint10*)last_item)->y;

  ic_dx->compress(median_x, x_diff, last_dir);
  // we use the number k of bits corrector bits to switch contexts
  U32 k_bits = ic_dx->getK();
  ic_dy->compress(median_y, y_diff, k_bits);
  k_bits = (k_bits + ic_dy->getK()) / 2;
  ic_z->compress(((LASpoint10*)last_item)->z, ((LASpoint10*)item)->z, k_bits);

  // compress which other values have changed
  I32 changed_values = ((((LASpoint10*)last_item)->intensity != ((LASpoint10*)item)->intensity) << 5) |
                       ((last_item[14] != item[14]) << 4) |
                       ((((LASpoint10*)last_item)->classification != ((LASpoint10*)item)->classification) << 3) |
                       ((((LASpoint10*)last_item)->scan_angle_rank != ((LASpoint10*)item)->scan_angle_rank) << 2) |
                       ((((LASpoint10*)last_item)->user_data != ((LASpoint10*)item)->user_data) << 1) |
                       ((((LASpoint10*)last_item)->point_source_ID != ((LASpoint10*)item)->point_source_ID));

  enc->encodeSymbol(m_changed_values, changed_values);

  // compress the intensity if it has changed
  if (changed_values & 32)
  {
    ic_intensity->compress(((LASpoint10*)last_item)->intensity, ((LASpoint10*)item)->intensity);
  }

  // compress the edge_of_flight_line, scan_direction_flag, ... if it has changed
  if (changed_values & 16)
  {
    enc->encodeSymbol(m_bit_byte, item[14]);
  }

  // compress the classification ... if it has changed
  if (changed_values & 8)
  {
    enc->encodeSymbol(m_classification, ((LASpoint10*)item)->classification);
  }
  
  // compress the scan_angle_rank ... if it has changed
  if (changed_values & 4)
  {
    ic_scan_angle_rank->compress(((LASpoint10*)last_item)->scan_angle_rank, ((LASpoint10*)item)->scan_angle_rank, k_bits < 3);
  }

  // compress the user_data ... if it has changed
  if (changed_values & 2)
  {
    enc->encodeSymbol(m_user_data, ((LASpoint10*)item)->user_data);
  }

  // compress the point_source_ID ... if it has changed
  if (changed_values & 1)
  {
    ic_point_source_ID->compress(((LASpoint10*)last_item)->point_source_ID, ((LASpoint10*)item)->point_source_ID);
  }

  // only record the difference if the scan direction has not changed
  if (last_dir == ((LASpoint10*)item)->scan_direction_flag)
  {
    last_x_diff[last_dir][last_incr[last_dir]] = x_diff;
    last_y_diff[last_dir][last_incr[last_dir]] = y_diff;
    last_incr[last_dir]++;
    if (last_incr[last_dir] > 2) last_incr[last_dir] = 0;
  }
  else
  {
    last_dir = ((LASpoint10*)item)->scan_direction_flag;
  }

  // copy the last item
  memcpy(last_item, item, 20);
  return TRUE;
}

/*
===============================================================================
                       LASwriteItemCompressed_GPSTIME_v1
===============================================================================
*/

#define LASZIP_GPSTIME_MULTIMAX 512

LASwriteItemCompressed_GPSTIME_v1::LASwriteItemCompressed_GPSTIME_v1(EntropyEncoder* enc)
{
  /* set encoder */
  assert(enc);
  this->enc = enc;
  /* create entropy models and integer compressors */
  m_gpstime_multi = enc->createSymbolModel(LASZIP_GPSTIME_MULTIMAX);
  m_gpstime_0diff = enc->createSymbolModel(3);
  ic_gpstime = new IntegerCompressor(enc, 32, 6); // 32 bits, 6 contexts
}

LASwriteItemCompressed_GPSTIME_v1::~LASwriteItemCompressed_GPSTIME_v1()
{
  enc->destroySymbolModel(m_gpstime_multi);
  delete ic_gpstime;
}

BOOL LASwriteItemCompressed_GPSTIME_v1::init(U8* item)
{
  /* init state */
  last_gpstime_diff = 0;
  multi_extreme_counter = 0;

  /* init models and integer compressors */
  enc->initSymbolModel(m_gpstime_multi);
  enc->initSymbolModel(m_gpstime_0diff);
  ic_gpstime->initCompressor();

  /* init last item */
  last_gpstime.f64 = *((F64*)item);
  return TRUE;
}

BOOL LASwriteItemCompressed_GPSTIME_v1::write(U8* item)
{
  I64F64 this_gpstime;
  this_gpstime.f64 = *((F64*)item);

  if (last_gpstime_diff == 0) // if the last integer difference was zero
  {
    if (this_gpstime.i64 == last_gpstime.i64)
    {
      enc->encodeSymbol(m_gpstime_0diff, 0); // the doubles have not changed
    }
    else
    {
      // calculate the difference between the two doubles as an integer
      I64 curr_gpstime_diff_64 = this_gpstime.i64 - last_gpstime.i64;
      I32 curr_gpstime_diff = (I32)curr_gpstime_diff_64;
      if (curr_gpstime_diff_64 == (I64)(curr_gpstime_diff))
      {
        enc->encodeSymbol(m_gpstime_0diff, 1); // the difference can be represented with 32 bits
        ic_gpstime->compress(0, curr_gpstime_diff, 0);
        last_gpstime_diff = curr_gpstime_diff;
      }
      else
      {
        enc->encodeSymbol(m_gpstime_0diff, 2); // the difference is huge
        enc->writeDouble(this_gpstime.f64);
      }
      last_gpstime.f64 = this_gpstime.f64;
    }
  }
  else // the last integer difference was *not* zero
  {
    if (this_gpstime.i64 == last_gpstime.i64)
    {
      // if the doubles have not changed use a special symbol
      enc->encodeSymbol(m_gpstime_multi, LASZIP_GPSTIME_MULTIMAX-1);
    }
    else
    {
      // calculate the difference between the two doubles as an integer
      I64 curr_gpstime_diff_64 = this_gpstime.i64 - last_gpstime.i64;
      I32 curr_gpstime_diff = (I32)curr_gpstime_diff_64;
      // if the current gpstime difference can be represented with 32 bits
      if (curr_gpstime_diff_64 == (I64)(curr_gpstime_diff))
      {
        // compute multiplier between current and last integer difference
        I32 multi = (I32)(((F32)curr_gpstime_diff / (F32)last_gpstime_diff) + 0.5f);
        // limit the multiplier into some bounds
        if (multi >= LASZIP_GPSTIME_MULTIMAX-3)
        {
          multi = LASZIP_GPSTIME_MULTIMAX-3;
        }
        else if (multi <= 0)
        {
          multi = 0;
        }
        // compress this multiplier
        enc->encodeSymbol(m_gpstime_multi, multi);
        // compress the residual curr_gpstime_diff in dependance on the multiplier
	      if (multi == 1)
        {
          // this is the case we assume we get most often
	        ic_gpstime->compress(last_gpstime_diff, curr_gpstime_diff, 1);
          last_gpstime_diff = curr_gpstime_diff;
          multi_extreme_counter = 0;	  
	      }
	      else
        {
          if (multi == 0)
          {
            ic_gpstime->compress(last_gpstime_diff/4, curr_gpstime_diff, 2);
        	  multi_extreme_counter++;
	          if (multi_extreme_counter > 3)
	          {
	            last_gpstime_diff = curr_gpstime_diff;
	            multi_extreme_counter = 0;
	          }
          }
	        else if (multi < 10)
          {
            ic_gpstime->compress(multi*last_gpstime_diff, curr_gpstime_diff, 3);
          }
          else if (multi < 50)
          {
            ic_gpstime->compress(multi*last_gpstime_diff, curr_gpstime_diff, 4);
	        }
          else
          {
            ic_gpstime->compress(multi*last_gpstime_diff, curr_gpstime_diff, 5);
	          if (multi == LASZIP_GPSTIME_MULTIMAX-3)
	          {
	            multi_extreme_counter++;
	            if (multi_extreme_counter > 3)
	            {
	              last_gpstime_diff = curr_gpstime_diff;
	              multi_extreme_counter = 0;
	            }
	          }
          }
        }
      }
      else
      {
        // if difference is so huge ... we simply write the double (for now) 
        enc->encodeSymbol(m_gpstime_multi, LASZIP_GPSTIME_MULTIMAX-2);
        enc->writeDouble(this_gpstime.f64);
      }
      last_gpstime.f64 = this_gpstime.f64;
    }
  }
  return TRUE;
}

/*
===============================================================================
                       LASwriteItemCompressed_RGB_v1
===============================================================================
*/

LASwriteItemCompressed_RGB_v1::LASwriteItemCompressed_RGB_v1(EntropyEncoder* enc)
{
  /* set encoder */
  assert(enc);
  this->enc = enc;

  /* create models and integer compressors */
  ic_rgb = new IntegerCompressor(enc, 16, 3); // 16 bits, 3 contexts

  /* create last item */
  last_item = new U8[6];
}

LASwriteItemCompressed_RGB_v1::~LASwriteItemCompressed_RGB_v1()
{
  delete ic_rgb;
  delete [] last_item;
}

BOOL LASwriteItemCompressed_RGB_v1::init(U8* item)
{
  /* init state */

  /* init models and integer compressors */
  ic_rgb->initCompressor();

  /* init last item */
  memcpy(last_item, item, 6);
  return TRUE;
}

BOOL LASwriteItemCompressed_RGB_v1::write(U8* item)
{
	ic_rgb->compress(((U16*)last_item)[0], ((U16*)item)[0], 0);
	ic_rgb->compress(((U16*)last_item)[1], ((U16*)item)[1], 1);
	ic_rgb->compress(((U16*)last_item)[2], ((U16*)item)[2], 2);
  memcpy(last_item, item, 6);
  return TRUE;
}

/*
===============================================================================
                       LASwriteItemCompressed_BYTE_v1
===============================================================================
*/

LASwriteItemCompressed_BYTE_v1::LASwriteItemCompressed_BYTE_v1(EntropyEncoder* enc, U32 number)
{
  /* set encoder */
  assert(enc);
  this->enc = enc;
  assert(number);
  this->number = number;

  /* create models and integer compressors */
  ic_byte = new IntegerCompressor(enc, 8, number);

  /* create last item */
  last_item = new U8[number];
}

LASwriteItemCompressed_BYTE_v1::~LASwriteItemCompressed_BYTE_v1()
{
  delete ic_byte;
  delete [] last_item;
}

BOOL LASwriteItemCompressed_BYTE_v1::init(U8* item)
{
  /* init state */

  /* init models and integer compressors */
  ic_byte->initCompressor();

  /* init last point */
  memcpy(last_item, item, number);
  return TRUE;
}

BOOL LASwriteItemCompressed_BYTE_v1::write(U8* item)
{
  U32 i;
  for (i = 0; i < number; i++)
  {
  	ic_byte->compress(last_item[i], item[i], i);
  }
  memcpy(last_item, item, number);
  return TRUE;
}
