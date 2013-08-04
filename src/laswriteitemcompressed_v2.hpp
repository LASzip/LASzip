/*
===============================================================================

  FILE:  laswriteitemcompressed_v2.hpp
  
  CONTENTS:
  
    Implementation of LASitemReadCompressed for *all* items (version 2).

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
  
    5 March 2011 -- created first night in ibiza to improve the RGB compressor

===============================================================================
*/
#ifndef LAS_WRITE_ITEM_COMPRESSED_V2_HPP
#define LAS_WRITE_ITEM_COMPRESSED_V2_HPP

#include "laswriteitem.hpp"
#include "entropyencoder.hpp"
#include "integercompressor.hpp"

#include "laszip_common_v2.hpp"

class LASwriteItemCompressed_POINT10_v2 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_POINT10_v2(EntropyEncoder* enc);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_POINT10_v2();

private:
  EntropyEncoder* enc;
  U8 last_item[20];
  U16 last_intensity[16];
  StreamingMedian5 last_x_diff_median5[16];
  StreamingMedian5 last_y_diff_median5[16];
  I32 last_height[8];

  EntropyModel* m_changed_values;
  IntegerCompressor* ic_intensity;
  EntropyModel* m_scan_angle_rank[2];
  IntegerCompressor* ic_point_source_ID;
  EntropyModel* m_bit_byte[256];
  EntropyModel* m_classification[256];
  EntropyModel* m_user_data[256];
  IntegerCompressor* ic_dx;
  IntegerCompressor* ic_dy;
  IntegerCompressor* ic_z;
};

class LASwriteItemCompressed_GPSTIME11_v2 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_GPSTIME11_v2(EntropyEncoder* enc);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_GPSTIME11_v2();

private:
  EntropyEncoder* enc;
  U32 last, next;
  U64I64F64 last_gpstime[4];
  I32 last_gpstime_diff[4];
  I32 multi_extreme_counter[4];

  EntropyModel* m_gpstime_multi;
  EntropyModel* m_gpstime_0diff;
  IntegerCompressor* ic_gpstime;
};

class LASwriteItemCompressed_RGB12_v2 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_RGB12_v2(EntropyEncoder* enc);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_RGB12_v2();

private:
  EntropyEncoder* enc;
  U16 last_item[3];

  EntropyModel* m_byte_used;
  EntropyModel* m_rgb_diff_0;
  EntropyModel* m_rgb_diff_1;
  EntropyModel* m_rgb_diff_2;
  EntropyModel* m_rgb_diff_3;
  EntropyModel* m_rgb_diff_4;
  EntropyModel* m_rgb_diff_5;
};

class LASwriteItemCompressed_BYTE_v2 : public LASwriteItemCompressed
{
public:

  LASwriteItemCompressed_BYTE_v2(EntropyEncoder* enc, U32 number);

  BOOL init(const U8* item);
  BOOL write(const U8* item);

  ~LASwriteItemCompressed_BYTE_v2();

private:
  EntropyEncoder* enc;
  U32 number;
  U8* last_item;

  EntropyModel** m_byte;
};

#endif
