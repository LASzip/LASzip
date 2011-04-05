/*
===============================================================================

  FILE:  laszip.hpp
  
  CONTENTS:
  
    Contains LASitem and LASchunk structs as well as the IDs of the currently
    supported entropy coding scheme

  PROGRAMMERS:
  
    martin.isenburg@gmail.com
  
  COPYRIGHT:

    (c) 2010-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    20 March 2011 -- incrementing LASZIP_VERSION to 2.0 for improved compression
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- refactored from lasdefinitions after movies with silke
  
===============================================================================
*/
#ifndef LASZIP_HPP
#define LASZIP_HPP

#if defined(_MSC_VER) && (_MSC_VER < 1300)
#define LZ_WIN32_VC6
typedef __int64   SIGNED_INT64;
#else
typedef long long SIGNED_INT64;
#endif

#define LASZIP_VERSION_MAJOR    1
#define LASZIP_VERSION_MINOR    2
#define LASZIP_VERSION_REVISION 0

#include "laszipexport.hpp"

class LASZIP_DLL LASitem
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

  const char* get_name() const;

public:
  unsigned short size;
  unsigned short version;
};

class LASZIP_DLL LASzip
{
public:

  enum Algorithm
  {
    POINT_BY_POINT_RAW        = 0,
    POINT_BY_POINT_ARITHMETIC = 1,
    POINT_BY_POINT_ARITHMETIC_V2 = 2, // temporary fix
    DEFAULT_COMPRESSION       = POINT_BY_POINT_ARITHMETIC,
    DEFAULT_COMPRESSION_V2    = POINT_BY_POINT_ARITHMETIC_V2, // temporary fix
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
