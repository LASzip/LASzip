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

  FILE:  lasreadpoint.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-10  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/

#include "lasreadpoint.hpp"

#include "arithmeticdecoder.hpp"
#include "lasreaditemraw.hpp"
#include "lasreaditemrawendianswapped.hpp"
#include "lasreaditemcompressed_v1.hpp"

#include <string.h>

LASreadPoint::LASreadPoint()
{
  instream = 0;
  num_readers = 0;
  readers = 0;
  readers_raw = 0;
  readers_compressed = 0;
  dec = 0;
}

BOOL LASreadPoint::setup(U32 num_items, const LASitem* items, U32 compression)
{
  U32 i;

  // check if we support the items
  for (i = 0; i < num_items; i++)
  {
    switch (items[i].type)
    {
    case LASitem::POINT10:
      if (items[i].size != 20) return FALSE;
      break;
    case LASitem::GPSTIME:
      if (items[i].size != 8) return FALSE;
      break;
    case LASitem::RGB:
      if (items[i].size != 6) return FALSE;
      break;
    case LASitem::BYTE:
      if (items[i].size < 1) return FALSE;
      break;
    default:
      return FALSE;
    }
  }

  // check if we support the compression
  if (compression)
  {
    // check if we support the entropy decoder
    switch (compression)
    {
    case LASZIP_COMPRESSION_RANGE:
    case LASZIP_COMPRESSION_ARITHMETIC:
      break;
    default:
      return FALSE;
    }

    // check if we support the compressor versions
    for (i = 0; i < num_items; i++)
    {
      switch (items[i].type)
      {
      case LASitem::POINT10:
        if ((items[i].version < 1) || (items[i].version > 1)) return FALSE;
        break;
      case LASitem::GPSTIME:
        if ((items[i].version < 1) || (items[i].version > 1)) return FALSE;
        break;
      case LASitem::RGB:
        if ((items[i].version < 1) || (items[i].version > 1)) return FALSE;
        break;
      case LASitem::BYTE:
        if ((items[i].version < 1) || (items[i].version > 1)) return FALSE;
        break;
      default:
        return FALSE;
      }
    }
  }

  // create entropy decoder (if requested)
  switch (compression)
  {
  case LASZIP_COMPRESSION_NONE:
    dec = 0;
    break;
//  case LASZIP_COMPRESSION_RANGE:
//    enc = new RangeDecoder();
//    break;
  case LASZIP_COMPRESSION_ARITHMETIC:
    dec = new ArithmeticDecoder();
    break;
  default:
    return FALSE;
  }
 
  // initizalize the readers
  readers = 0;
  num_readers = num_items;

  // always create the raw readers
  readers_raw = new LASreadItem*[num_readers];
  for (i = 0; i < num_readers; i++)
  {
    switch (items[i].type)
    {
    case LASitem::POINT10:
      if (IS_LITTLE_ENDIAN())
        readers_raw[i] = new LASreadItemRaw_POINT10();
      else
        readers_raw[i] = new LASreadItemRawEndianSwapped_POINT10();
      break;
    case LASitem::GPSTIME:
      if (IS_LITTLE_ENDIAN())
        readers_raw[i] = new LASreadItemRaw_GPSTIME();
      else
        readers_raw[i] = new LASreadItemRawEndianSwapped_GPSTIME();
      break;
    case LASitem::RGB:
      if (IS_LITTLE_ENDIAN())
        readers_raw[i] = new LASreadItemRaw_RGB();
      else
        readers_raw[i] = new LASreadItemRawEndianSwapped_RGB();
      break;
    case LASitem::BYTE:
      readers_raw[i] = new LASreadItemRaw_BYTE(items[i].size);
      break;
    }
  }

  if (dec)
  {
    readers_compressed = new LASreadItem*[num_readers];
    for (i = 0; i < num_readers; i++)
    {
      switch (items[i].type)
      {
      case LASitem::POINT10:
        readers_compressed[i] = new LASreadItemCompressed_POINT10_v1(dec);
        break;
      case LASitem::GPSTIME:
        readers_compressed[i] = new LASreadItemCompressed_GPSTIME_v1(dec);
        break;
      case LASitem::RGB:
        readers_compressed[i] = new LASreadItemCompressed_RGB_v1(dec);
        break;
      case LASitem::BYTE:
        readers_compressed[i] = new LASreadItemCompressed_BYTE_v1(dec, items[i].size);
        break;
      }
    }
  }
  return TRUE;
}

BOOL LASreadPoint::init(ByteStreamIn* instream)
{
  if (!instream) return FALSE;
  U32 i;
  for (i = 0; i < num_readers; i++)
  {
    ((LASreadItemRaw*)(readers_raw[i]))->init(instream);
  }
  if (dec)
  {
    readers = 0;
    this->instream = instream;
  }
  else
  {
    readers = readers_raw;
  }
  return TRUE;
}

BOOL LASreadPoint::read(U8** point)
{
  U32 i;
  if (readers)
  {
    for (i = 0; i < num_readers; i++)
    {
      readers[i]->read(point[i]);
    }
  }
  else
  {
    for (i = 0; i < num_readers; i++)
    {
      readers_raw[i]->read(point[i]);
      ((LASreadItemCompressed*)(readers_compressed[i]))->init(point[i]);
    }
    readers = readers_compressed;
    dec->init(instream);
  }
  return TRUE;
}

LASreadPoint::~LASreadPoint()
{
  U32 i;

  if (readers_raw)
  {
    for (i = 0; i < num_readers; i++)
    {
      delete readers_raw[i];
    }
  }
  if (readers_compressed)
  {
    for (i = 0; i < num_readers; i++)
    {
      delete readers_compressed[i];
    }
  }
  if (dec)
  {
    delete dec;
  }
}
