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
}

bool LASzip::setup(const unsigned int num_items, const LASitem* items, const unsigned short compressor)
{
  unsigned int i;
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

void LASzip::set_chunk_size(const unsigned int chunk_size)
{
  this->chunk_size = chunk_size;
}

void LASzip::request_version(const unsigned int requested_version)
{
  this->requested_version = requested_version;
}

LASzip::~LASzip()
{
  if (items) delete [] items;
}

void LASitem::set(LASitem::Type t, unsigned short number)
{
  switch (t)
  {
  case LASitem::POINT10:
      type = LASitem::POINT10;
      size = 20;
      break;
  case LASitem::GPSTIME11:
      type = LASitem::GPSTIME11;
      size = 8;
      break;
  case LASitem::RGB12:
      type = LASitem::RGB12;
      size = 6;
      break;
  case LASitem::WAVEPACKET13:
      type = LASitem::WAVEPACKET13;
      size = 29;
      break;
  case LASitem::BYTE:
      type = LASitem::BYTE;
      size = number;
      break;
  default:
      throw 0; // BUG
  }
  return;
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

bool LASitem::supported_type() const
{
  switch (type)
  {
  case POINT10:
  case GPSTIME11:
  case RGB12:
  case WAVEPACKET13:
  case BYTE:
      return true;
      break;
  default:
      return false;
  }
  return false;
}

bool LASitem::supported_size() const
{
  switch (type)
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

bool LASitem::supported_version() const
{
  switch (type)
  {
  case POINT10:
      if (version > 2) return false;
      break;
  case GPSTIME11:
      if (version > 2) return false;
      break;
  case RGB12:
      if (version > 2) return false;
      break;
  case WAVEPACKET13:
      if (version > 1) return false;
      break;
  case BYTE:
      if (version > 2) return false;
      break;
  default:
      return false;
  }
  return true;
}

bool LASitem::supported() const
{
  return supported_type() && supported_size() && supported_version();
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
