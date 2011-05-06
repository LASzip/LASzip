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

#define LASZIP_COMPRESSOR_NONE              0
#define LASZIP_COMPRESSOR_POINTWISE         1
#define LASZIP_COMPRESSOR_POINTWISE_CHUNKED 2

#define LASZIP_COMPRESSOR_DEFAULT LASZIP_COMPRESSOR_POINTWISE
#define LASZIP_COMPRESSOR_CHUNKED LASZIP_COMPRESSOR_POINTWISE_CHUNKED

#define LASZIP_CODER_ARITHMETIC             0

#define LASZIP_CHUNK_SIZE_DEFAULT 50000

#include "laszipexport.hpp"

class LASZIP_DLL LASitem
{
public:
  enum Type { BYTE = 0, SHORT, INT, LONG, FLOAT, DOUBLE, POINT10, GPSTIME11, RGB12, WAVEPACKET13 } type;
  unsigned short size;
  unsigned short version;

  // number parameter only used when setting to BYTE
  void set(LASitem::Type t, unsigned short number=1);
  bool is_type(LASitem::Type t) const;
  bool supported() const;

  const char* get_name() const;
};

class LASZIP_DLL LASzip
{
public:

  LASzip();
  ~LASzip();

  // pack to and unpack from VLR
  unsigned char* bytes;
  bool unpack(const unsigned char* bytes, const int num);
  bool pack(unsigned char*& bytes, int& num);

  // setup
  bool setup(const unsigned char point_type, const unsigned short point_size, const unsigned short compressor=LASZIP_COMPRESSOR_DEFAULT);
  bool setup(const unsigned short num_items, const LASitem* items, const unsigned short compressor=LASZIP_COMPRESSOR_DEFAULT);
  void set_chunk_size(const unsigned int chunk_size);
  void request_version(const unsigned int requested_version);

  // stored in LASzip VLR data section
  unsigned short compressor;
  unsigned short coder;
  unsigned char version_major;
  unsigned char version_minor;
  unsigned short version_revision;
  unsigned int options;
  unsigned int chunk_size; 
  SIGNED_INT64 num_points;  /* not mandatory ... -1 if unknown */
  SIGNED_INT64 num_bytes;   /* not mandatory ... -1 if unknown */
  unsigned short num_items;
  LASitem* items;

  // not stored LASzip VLR data section
  unsigned short requested_version;
};

#endif
