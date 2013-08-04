/*
===============================================================================

  FILE:  laswriteitemcompressed_v1.hpp
  
  CONTENTS:
  
    Implementation of LASitemReadCompressed for *all* items (version 1).

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2007-2012, martin isenburg, rapidlasso - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
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
  U64I64F64 last_gpstime;

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
  U32 sym_last_offset_diff;
  EntropyModel* m_packet_index;
  EntropyModel* m_offset_diff[4];
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
