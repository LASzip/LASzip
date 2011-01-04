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

//#include "export.hpp"
#define LAS_ZIP_DLL

class LAS_ZIP_DLL LASitem
{
public:
  enum Type { BYTE = 0, SHORT, INT, LONG, FLOAT, DOUBLE, POINT10, GPSTIME11, RGB12, WAVEPACKET13 } type;

  // number parameter only used when setting to BYTE
  void set(LASitem::Type t, unsigned short number=1);

  bool is_type(LASitem::Type t) const;
  bool supported_type() const;
  bool supported_size() const;
  bool supported_version() const;
  bool supported() const;

public:
  unsigned short size;
  unsigned short version;
};

class LAS_ZIP_DLL LASzip
{
public:

  enum Algorithm
  {
    POINT_BY_POINT_RAW        = 0,
    POINT_BY_POINT_ARITHMETIC = 1,
    POINT_BY_POINT_RANGE      = 2,
    DEFAULT_COMPRESSION       = POINT_BY_POINT_ARITHMETIC,
    DEFAULT_RAW               = POINT_BY_POINT_RAW
  };

  LASzip();
  ~LASzip();

  Algorithm algorithm;
  unsigned char version_major;
  unsigned char version_minor;
  unsigned short version_revision;
  unsigned int options;
  unsigned int num_items;
  unsigned int num_chunks;  /* not used yet  ...               */ 
  SIGNED_INT64 num_points;  /* not mandatory ... -1 if unknown */
  SIGNED_INT64 num_bytes;   /* not mandatory ... -1 if unknown */
  LASitem* items;
};

/*
struct LASchunk
{
  unsigned int count;
  unsigned int bytes;
}; 
*/

#endif
