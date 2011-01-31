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
  algorithm = POINT_BY_POINT_RAW;
  version_major = 1;
  version_minor = 0;
  version_revision = 0;
  options = 0;
  num_items = 0;
  num_chunks = 1;
  num_points = -1;
  num_bytes = -1;
  items = 0;
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
      version = 1;
      break;
  case LASitem::GPSTIME11:
      type = LASitem::GPSTIME11;
      size = 8;
      version = 1;
      break;
  case LASitem::RGB12:
      type = LASitem::RGB12;
      size = 6;
      version = 1;
      break;
  case LASitem::WAVEPACKET13:
      type = LASitem::WAVEPACKET13;
      size = 29;
      version = 0;
      break;
  case LASitem::BYTE:
      type = LASitem::BYTE;
      size = number;
      version = 1;
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
      if (version > 1) return false;
      break;
  case GPSTIME11:
      if (version > 1) return false;
      break;
  case RGB12:
      if (version > 1) return false;
      break;
  case WAVEPACKET13:
      if (version > 1) return false;
      break;
  case BYTE:
      if (version > 1) return false;
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
      if (version > 1) return false;
      break;
  case GPSTIME11:
      if (size != 8) return false;
      if (version > 1) return false;
      break;
  case RGB12:
      if (size != 6) return false;
      if (version > 1) return false;
      break;
  case WAVEPACKET13:
      if (size != 29) return false;
      if (version > 1) return false;
      break;
  case BYTE:
      if (size < 1) return false;
      if (version > 1) return false;
      break;
  default:
      return false;
  }
  return true;
}
