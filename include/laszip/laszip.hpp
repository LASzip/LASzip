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

  FILE:  laszip.hpp
  
  CONTENTS:
  
    Contains LASitem and LASchunk structs as well as the IDs of the currently
    supported entropy coding scheme

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    12 December 2010 -- refactored from lasdefinitions after movies with silke
  
===============================================================================
*/
#ifndef LASZIP_H
#define LASZIP_H

#if defined(_MSC_VER) && (_MSC_VER < 1300)
#define LZ_WIN32_VC6
typedef __int64   SIGNED_INT64;
#else
typedef long long SIGNED_INT64;
#endif

#define LASZIP_VERSION_MAJOR    1
#define LASZIP_VERSION_MINOR    0
#define LASZIP_VERSION_REVISION 0

#define LASZIP_COMPRESSION_NONE       0
#define LASZIP_COMPRESSION_ARITHMETIC 1
#define LASZIP_COMPRESSION_RANGE      2

#define LASZIP_COMPRESSION_DEFAULT    1

class LASitem
{
public:

  enum Type { BYTE = 0, SHORT, INT, LONG, FLOAT, DOUBLE, POINT10, GPSTIME11, RGB12, WAVEPACKET13 } type;
  unsigned short size;
  unsigned short version;

  void set(LASitem::Type t, unsigned int number=1)
  {
    switch (t)
    {
    case LASitem::POINT10:
        type = LASitem::POINT10;
        size = 20;
        version = 0;
        break;
    case LASitem::GPSTIME11:
        type = LASitem::GPSTIME11;
        size = 8;
        version = 0;
        break;
    case LASitem::RGB12:
        type = LASitem::RGB12;
        size = 6;
        version = 0;
        break;
    case LASitem::WAVEPACKET13:
        type = LASitem::WAVEPACKET13;
        size = 29;
        version = 0;
        break;
    case LASitem::BYTE:
        type = LASitem::BYTE;
        size = number;
        version = 0;
        break;
    default:
        throw 0; // BUG
    }
    return;
  }

  bool is_type(LASitem::Type t) const
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

  bool supported_type() const
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

  bool supported_size() const
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

  bool supported_version() const
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

  bool supported() const
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
};

class LASzip
{
public:
  unsigned int compression;
  unsigned char version_major;
  unsigned char version_minor;
  unsigned short version_revision;
  unsigned int options;
  unsigned int num_items;
  unsigned int num_chunks;  /* not used yet  ...               */ 
  SIGNED_INT64 num_points;  /* not mandatory ... -1 if unknown */
  SIGNED_INT64 num_bytes;   /* not mandatory ... -1 if unknown */
  LASitem* items;

  LASzip()
  {
    compression = 0;
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

  ~LASzip()
  {
    if (items) delete [] items;
  }
};

/*
struct LASchunk
{
  unsigned int count;
  unsigned int bytes;
}; 
*/

#endif
