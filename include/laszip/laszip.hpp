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
#endif

#define LASZIP_VERSION_MAJOR    1
#define LASZIP_VERSION_MINOR    0
#define LASZIP_VERSION_REVISION 0

#define LASZIP_COMPRESSION_NONE 0
#define LASZIP_COMPRESSION_RANGE 1
#define LASZIP_COMPRESSION_ARITHMETIC 2

class LASitem
{
public:

  enum { BYTE = 0, SHORT, INT, LONG, FLOAT, DOUBLE, POINT10, GPSTIME, RGB, WAVEPACKET } type;
  unsigned short size;
  unsigned short version;

  bool supported_type() const
  {
    switch (type)
    {
    case POINT10:
    case GPSTIME:
    case RGB:
    case WAVEPACKET:
    case BYTE:
      return true;
      break;
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
    case GPSTIME:
      if (size != 8) return false;
      break;
    case RGB:
      if (size != 6) return false;
      break;
    case WAVEPACKET:
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
    case GPSTIME:
      if (version > 1) return false;
      break;
    case RGB:
      if (version > 1) return false;
      break;
    case WAVEPACKET:
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
    case GPSTIME:
      if (size != 8) return false;
      if (version > 1) return false;
      break;
    case RGB:
      if (size != 6) return false;
      if (version > 1) return false;
      break;
    case WAVEPACKET:
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

struct LASchunk
{
  unsigned int count;
  unsigned int bytes;
};

#endif
