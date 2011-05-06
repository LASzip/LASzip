/*
===============================================================================

  FILE:  laszip.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:

    martin.isenburg@gmail.com

  COPYRIGHT:

    (c) 2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/
#include "laszip.hpp"
#include "mydefs.hpp"
#include <assert.h>

LASzip::LASzip()
{
  compressor = LASZIP_COMPRESSOR_DEFAULT;
  coder = LASZIP_CODER_ARITHMETIC;
  version_major = LASZIP_VERSION_MAJOR;
  version_minor = LASZIP_VERSION_MINOR;
  version_revision = LASZIP_VERSION_REVISION;
  options = 0;
  num_items = 0;
  chunk_size = 0;
  num_points = -1;
  num_bytes = -1;
  items = 0;
  requested_version = 0;
  bytes = 0;
}

LASzip::~LASzip()
{
  if (items) delete [] items;
  if (bytes) delete [] bytes;
}

// unpack from VLR data
bool LASzip::unpack(const U8* bytes, const I32 num)
{
  if (num < 34) return false; // too few bytes
  if (((num - 34) % 6) != 0) return false; // wrong number bytes
  if (((num - 34) / 6) == 0) return false; // too few items
  num_items = (num - 34) / 6;
  if (items) delete [] items;
  items = new LASitem[num_items];

  // the data of the LASzip VLR
  //     U16  compressor         2 bytes 
  //     U16  coder              2 bytes 
  //     U8   version_major      1 byte 
  //     U8   version_minor      1 byte
  //     U16  version_revision   2 bytes
  //     U32  options            4 bytes 
  //     U32  chunk_size         4 bytes
  //     I64  num_points         8 bytes
  //     I64  num_bytes          8 bytes
  //     U16  num_items          2 bytes
  //        U16 type                2 bytes * num_items
  //        U16 size                2 bytes * num_items
  //        U16 version             2 bytes * num_items
  // which totals 34+6*num_items

  U16 i;
  const U8* b = bytes;
  compressor = *((U16*)b);
  b += 2;
  coder = *((U16*)b);
  b += 2;
  version_major = *((U8*)b);
  b += 1;
  version_minor = *((U8*)b);
  b += 1;
  version_revision = *((U16*)b);
  b += 2;
  options = *((U32*)b);
  b += 4;
  chunk_size = *((U32*)b);
  b += 4;
  num_points = *((I64*)b);
  b += 8;
  num_bytes = *((I64*)b);
  b += 8;
  num_items = *((U16*)b);
  b += 2;
  for (i = 0; i < num_items; i++)
  {
    items[i].type = (LASitem::Type)*((U16*)b);
    b += 2;
    items[i].size = *((U16*)b);
    b += 2;
    items[i].version = *((U16*)b);
    b += 2;
  }
  assert((bytes + num) == b);
  for (i = 0; i < num_items; i++)
  {
    if (!items[i].supported()) return false;
  }
  return true;
}

// pack to VLR data
bool LASzip::pack(U8*& bytes, I32& num)
{
  num = 34 + 6*num_items;
  if (this->bytes) delete [] this->bytes;
  this->bytes = bytes = new U8[num];

  // the data of the LASzip VLR
  //     U16  compressor         2 bytes 
  //     U16  coder              2 bytes 
  //     U8   version_major      1 byte 
  //     U8   version_minor      1 byte
  //     U16  version_revision   2 bytes
  //     U32  options            4 bytes 
  //     U32  chunk_size         4 bytes
  //     I64  num_points         8 bytes
  //     I64  num_bytes          8 bytes
  //     U16  num_items          2 bytes
  //        U16 type                2 bytes * num_items
  //        U16 size                2 bytes * num_items
  //        U16 version             2 bytes * num_items
  // which totals 34+6*num_items

  U16 i;
  U8* b = bytes;
  *((U16*)b) = compressor;
  b += 2;
  *((U16*)b) = coder;
  b += 2;
  *((U8*)b) = version_major;
  b += 1;
  *((U8*)b) = version_minor;
  b += 1;
  *((U16*)b) = version_revision;
  b += 2;
  *((U32*)b) = options;
  b += 4;
  *((U32*)b) = chunk_size;
  b += 4;
  *((I64*)b) = num_points;
  b += 8;
  *((I64*)b) = num_bytes;
  b += 8;
  *((U16*)b) = num_items;
  b += 2;
  for (i = 0; i < num_items; i++)
  {
    *((U16*)b) = (U16)items[i].type;
    b += 2;
    *((U16*)b) = items[i].size;
    b += 2;
    *((U16*)b) = items[i].version;
    b += 2;
  }
  assert((bytes + num) == b);
  for (i = 0; i < num_items; i++)
  {
    if (!items[i].supported()) return false;
  }
  return true;
}

bool LASzip::setup(const U8 point_type, const U16 point_size, const U16 compressor)
{
  if (compressor > LASZIP_COMPRESSOR_POINTWISE_CHUNKED) return false;

  // switch over the point types we know

  BOOL have_gps_time = FALSE;
  BOOL have_rgb = FALSE;
  BOOL have_wavepacket = FALSE;
  I32 extra_bytes_number = 0;

  switch (point_type)
  {
  case 0:
    extra_bytes_number = (I32)point_size - 20;
    break;
  case 1:
    have_gps_time = TRUE;
    extra_bytes_number = (I32)point_size - 28;
    break;
  case 2:
    have_rgb = TRUE;
    extra_bytes_number = (I32)point_size - 26;
    break;
  case 3:
    have_gps_time = TRUE;
    have_rgb = TRUE;
    extra_bytes_number = (I32)point_size - 34;
    break;
  case 4:
    have_gps_time = TRUE;
    have_wavepacket = TRUE;
    extra_bytes_number = (I32)point_size - 57;
    break;
  case 5:
    have_gps_time = TRUE;
    have_rgb = TRUE;
    have_wavepacket = TRUE;
    extra_bytes_number = (I32)point_size - 63;
    break;
  default:
    return false;
  }

  if (extra_bytes_number < 0) return false;

  // create item description

  if (items) delete [] items;
  num_items = 1 + !!(have_gps_time) + !!(have_rgb) + !!(have_wavepacket) + !!(extra_bytes_number);
  items = new LASitem[num_items];

  U16 i = 1;
  items[0].type = LASitem::POINT10;
  items[0].size = 20;
  items[0].version = 0;
  if (have_gps_time)
  {
    items[i].type = LASitem::GPSTIME11;
    items[i].size = 8;
    items[i].version = 0;
    i++;
  }
  if (have_rgb)
  {
    items[i].type = LASitem::RGB12;
    items[i].size = 6;
    items[i].version = 0;
    i++;
  }
  if (have_wavepacket)
  {
    items[i].type = LASitem::WAVEPACKET13;
    items[i].size = 29;
    items[i].version = 0;
    i++;
  }
  if (extra_bytes_number)
  {
    items[i].type = LASitem::BYTE;
    items[i].size = extra_bytes_number;
    items[i].version = 0;
    i++;
  }
  assert(i == num_items);
  this->compressor = compressor;
  if (this->compressor == LASZIP_COMPRESSOR_POINTWISE_CHUNKED)
  {
    if (chunk_size == 0) chunk_size = LASZIP_CHUNK_SIZE_DEFAULT;
  }
  return true;
}

bool LASzip::setup(const U16 num_items, const LASitem* items, const U16 compressor)
{
  U16 i;
  if (num_items == 0) return false;
  if (items == 0) return false;
  if (compressor > LASZIP_COMPRESSOR_POINTWISE_CHUNKED) return false;
  for (i = 0; i < num_items; i++)
  {
    if (!items[i].supported()) return false;
  }
  this->num_items = num_items;
  if (this->items) delete [] this->items;
  this->items = new LASitem[num_items];
  for (i = 0; i < num_items; i++)
  {
    this->items[i] = items[i];
  }
  this->compressor = compressor;
  if (this->compressor == LASZIP_COMPRESSOR_POINTWISE_CHUNKED)
  {
    if (chunk_size == 0) chunk_size = LASZIP_CHUNK_SIZE_DEFAULT;
  }
  return true;
}

void LASzip::set_chunk_size(const U32 chunk_size)
{
  this->chunk_size = chunk_size;
}

void LASzip::request_version(const U32 requested_version)
{
  this->requested_version = requested_version;
}

bool LASitem::is_type(LASitem::Type t) const
{
  if (t != type) return false;
  switch (t)
  {
  case POINT10:
      if (size != 20) return false;
      break;
  case GPSTIME11:
      if (size != 8) return false;
      break;
  case RGB12:
      if (size != 6) return false;
      break;
  case WAVEPACKET13:
      if (size != 29) return false;
      break;
  case BYTE:
      if (size < 1) return false;
      break;
  default:
      return false;
  }
  return true;
}

bool LASitem::supported() const
{
  switch (type)
  {
  case POINT10:
      if (size != 20) return false;
      if (version > 2) return false;
      break;
  case GPSTIME11:
      if (size != 8) return false;
      if (version > 2) return false;
      break;
  case RGB12:
      if (size != 6) return false;
      if (version > 2) return false;
      break;
  case WAVEPACKET13:
      if (size != 29) return false;
      if (version > 1) return false;
      break;
  case BYTE:
      if (size < 1) return false;
      if (version > 2) return false;
      break;
  default:
      return false;
  }
  return true;
}

const char* LASitem::get_name() const
{
  switch (type)
  {
  case POINT10:
      return "POINT10";
      break;
  case GPSTIME11:
      return "GPSTIME11";
      break;
  case RGB12:
      return "RGB12";
      break;
  case WAVEPACKET13:
      return "WAVEPACKET13";
      break;
  case BYTE:
      return "BYTE";
      break;
  default:
      break;
  }
  return 0;
}
