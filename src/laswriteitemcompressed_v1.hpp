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

  FILE:  laswriteitemcompressed_v1.hpp
  
  CONTENTS:
  
    Implementation of LASitemReadCompressed for *all* items (version 1).

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    12 December 2010 -- refactored after watching two movies with silke
  
===============================================================================
*/
#ifndef LAS_WRITE_ITEM_COMPRESSED_V1_H
#define LAS_WRITE_ITEM_COMPRESSED_V1_H

#include "laswriteitem.hpp"
#include "entropyencoder.hpp"
#include "integercompressor.hpp"

class LASwriteItemCompressed_POINT10_v1 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_POINT10_v1(EntropyEncoder* enc);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_POINT10_v1();

private:
  EntropyEncoder* enc;
  U8 last_item[20];

  I32 last_x_diff[3];
  I32 last_y_diff[3];
  I32 last_incr;
  IntegerCompressor* ic_dx;
  IntegerCompressor* ic_dy;
  IntegerCompressor* ic_z;
  IntegerCompressor* ic_intensity;
  IntegerCompressor* ic_scan_angle_rank;
  IntegerCompressor* ic_point_source_ID;
  EntropyModel* m_changed_values;
  EntropyModel* m_bit_byte[256];
  EntropyModel* m_classification[256];
  EntropyModel* m_user_data[256];
};

class LASwriteItemCompressed_GPSTIME11_v1 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_GPSTIME11_v1(EntropyEncoder* enc);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_GPSTIME11_v1();

private:
  EntropyEncoder* enc;
  I64F64 last_gpstime;

  EntropyModel* m_gpstime_multi;
  EntropyModel* m_gpstime_0diff;
  IntegerCompressor* ic_gpstime;
  I32 multi_extreme_counter;
  I32 last_gpstime_diff;
};

class LASwriteItemCompressed_RGB12_v1 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_RGB12_v1(EntropyEncoder* enc);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_RGB12_v1();

private:
  EntropyEncoder* enc;
  U8* last_item;

  EntropyModel* m_byte_used;
  IntegerCompressor* ic_rgb;
};

class LASwriteItemCompressed_WAVEPACKET13_v1 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_WAVEPACKET13_v1(EntropyEncoder* enc);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_WAVEPACKET13_v1();

private:
  EntropyEncoder* enc;
  U8* last_item;

  I32 last_diff_32;
  EntropyModel* m_packet_index;
  EntropyModel* m_small_offset_diff;
  IntegerCompressor* ic_offset_diff;
  IntegerCompressor* ic_packet_size;
  IntegerCompressor* ic_return_point;
  IntegerCompressor* ic_xyz;
};

class LASwriteItemCompressed_BYTE_v1 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_BYTE_v1(EntropyEncoder* enc, U32 number);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_BYTE_v1();

private:
  EntropyEncoder* enc;
  U32 number;
  U8* last_item;

  IntegerCompressor* ic_byte;
};

#endif
