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

  FILE:  lasreaditemcompressed_v1.hpp
  
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
  
    7 December 2010 -- refactored after getting invited to KAUST in Saudi Arabia
  
===============================================================================
*/
#ifndef LAS_READ_ITEM_COMPRESSED_V1_H
#define LAS_READ_ITEM_COMPRESSED_V1_H

#include "lasreaditem.hpp"
#include "entropydecoder.hpp"
#include "integercompressor.hpp"

class LASreadItemCompressed_POINT10_v1 : public LASreadItemCompressed
{
public:

  LASreadItemCompressed_POINT10_v1(EntropyDecoder* dec);

  BOOL init(U8* item);
  BOOL read(U8* item);

  ~LASreadItemCompressed_POINT10_v1();

private:
  EntropyDecoder* dec;
  U8* last_item;

  I32 last_dir;
  I32 last_x_diff[2][3];
  I32 last_y_diff[2][3];
  I32 last_incr[2];
  IntegerCompressor* ic_dx;
  IntegerCompressor* ic_dy;
  IntegerCompressor* ic_z;
  EntropyModel* m_changed_values;
  IntegerCompressor* ic_intensity;
  EntropyModel* m_bit_byte;
  EntropyModel* m_classification;
  IntegerCompressor* ic_scan_angle_rank;
  EntropyModel* m_user_data;
  IntegerCompressor* ic_point_source_ID;
};

class LASreadItemCompressed_GPSTIME_v1 : public LASreadItemCompressed
{
public:

  LASreadItemCompressed_GPSTIME_v1(EntropyDecoder* dec);

  BOOL init(U8* item);
  BOOL read(U8* item);

  ~LASreadItemCompressed_GPSTIME_v1();

private:
  EntropyDecoder* dec;
  I64F64 last_gpstime;

  EntropyModel* m_gpstime_multi;
  EntropyModel* m_gpstime_0diff;
  IntegerCompressor* ic_gpstime;
  I32 multi_extreme_counter;
  I32 last_gpstime_diff;
};

class LASreadItemCompressed_RGB_v1 : public LASreadItemCompressed
{
public:

  LASreadItemCompressed_RGB_v1(EntropyDecoder* dec);

  BOOL init(U8* item);
  BOOL read(U8* item);

  ~LASreadItemCompressed_RGB_v1();

private:
  EntropyDecoder* dec;
  U8* last_item;

  EntropyModel* m_byte_used;
  IntegerCompressor* ic_rgb;
};

class LASreadItemCompressed_BYTE_v1 : public LASreadItemCompressed
{
public:

  LASreadItemCompressed_BYTE_v1(EntropyDecoder* dec, U32 number);

  BOOL init(U8* item);
  BOOL read(U8* item);

  ~LASreadItemCompressed_BYTE_v1();

private:
  EntropyDecoder* dec;
  U32 number;
  U8* last_item;

  IntegerCompressor* ic_byte;
};

#endif
