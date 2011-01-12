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

  FILE:  laswritepoint.cpp
  
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

#include "laswritepoint.hpp"

#include "arithmeticencoder.hpp"
#ifdef LASZIP_HAVE_RANGECODER
#include "rangeencoder.hpp"
#endif
#include "laswriteitemraw.hpp"
#include "laswriteitemcompressed_v1.hpp"

#include <string.h>

LASwritePoint::LASwritePoint()
{
  outstream = 0;
  num_writers = 0;
  writers = 0;
  writers_raw = 0;
  writers_compressed = 0;
  enc = 0;
}

BOOL LASwritePoint::setup(U32 num_items, LASitem* items, LASzip::Algorithm algorithm)
{
  U32 i;

  // check if we support the items
  for (i = 0; i < num_items; i++)
  {
    if (!items[i].supported()) return FALSE;
  }

  // create entropy encoder (if requested)
  switch (algorithm)
  {
  case LASzip::POINT_BY_POINT_RAW:
    enc = 0;
    break;
#ifdef LASZIP_HAVE_RANGECODER
  case LASzip::POINT_BY_POINT_RANGE:
    enc = new RangeEncoder();
    break;
#endif
  case LASzip::POINT_BY_POINT_ARITHMETIC:
    enc = new ArithmeticEncoder();
    break;
  default:
    // entropy decoder not supported
    return FALSE;
  }

  // initizalize the writers
  writers = 0;
  num_writers = num_items;

  // always create the raw writers
  writers_raw = new LASwriteItem*[num_writers];
  for (i = 0; i < num_writers; i++)
  {
    switch (items[i].type)
    {
    case LASitem::POINT10:
      if (IS_LITTLE_ENDIAN())
        writers_raw[i] = new LASwriteItemRaw_POINT10_LE();
      else
        writers_raw[i] = new LASwriteItemRaw_POINT10_BE();
      items[i].version = 0;
      break;
    case LASitem::GPSTIME11:
      if (IS_LITTLE_ENDIAN())
        writers_raw[i] = new LASwriteItemRaw_GPSTIME11_LE();
      else
        writers_raw[i] = new LASwriteItemRaw_GPSTIME11_BE();
      items[i].version = 0;
      break;
    case LASitem::RGB12:
      if (IS_LITTLE_ENDIAN())
        writers_raw[i] = new LASwriteItemRaw_RGB12_LE();
      else
        writers_raw[i] = new LASwriteItemRaw_RGB12_BE();
      items[i].version = 0;
      break;
    case LASitem::WAVEPACKET13:
      if (IS_LITTLE_ENDIAN())
        writers_raw[i] = new LASwriteItemRaw_WAVEPACKET13_LE();
      else
        writers_raw[i] = new LASwriteItemRaw_WAVEPACKET13_BE();
      items[i].version = 0;
      break;
    case LASitem::BYTE:
      writers_raw[i] = new LASwriteItemRaw_BYTE(items[i].size);
      items[i].version = 0;
      break;
    default:
      return FALSE;
    }
  }

  // if needed create the compressed writers and set versions
  if (enc)
  {
    writers_compressed = new LASwriteItem*[num_writers];
    for (i = 0; i < num_writers; i++)
    {
      switch (items[i].type)
      {
      case LASitem::POINT10:
        writers_compressed[i] = new LASwriteItemCompressed_POINT10_v1(enc);
        items[i].version = 1;
        break;
      case LASitem::GPSTIME11:
        writers_compressed[i] = new LASwriteItemCompressed_GPSTIME11_v1(enc);
        items[i].version = 1;
        break;
      case LASitem::RGB12:
        writers_compressed[i] = new LASwriteItemCompressed_RGB12_v1(enc);
        items[i].version = 1;
        break;
      case LASitem::WAVEPACKET13:
        writers_compressed[i] = new LASwriteItemCompressed_WAVEPACKET13_v1(enc);
        items[i].version = 1;
        break;
      case LASitem::BYTE:
        writers_compressed[i] = new LASwriteItemCompressed_BYTE_v1(enc, items[i].size);
        items[i].version = 1;
        break;
      default:
        return FALSE;
      }
    }
  }
  return TRUE;
}

BOOL LASwritePoint::init(ByteStreamOut* outstream)
{
  if (!outstream) return FALSE;

  U32 i;
  for (i = 0; i < num_writers; i++)
  {
    ((LASwriteItemRaw*)(writers_raw[i]))->init(outstream);
  }

  if (enc)
  {
    writers = 0;
    this->outstream = outstream;
  }
  else
  {
    writers = writers_raw;
  }
  return TRUE;
}

BOOL LASwritePoint::write(const U8 * const * point)
{
  U32 i;

  if (writers)
  {
    for (i = 0; i < num_writers; i++)
    {
      writers[i]->write(point[i]);
    }
  }
  else
  {
    for (i = 0; i < num_writers; i++)
    {
      writers_raw[i]->write(point[i]);
      ((LASwriteItemCompressed*)(writers_compressed[i]))->init(point[i]);
    }
    writers = writers_compressed;
    enc->init(outstream);
  }
  return TRUE;
}

BOOL LASwritePoint::done()
{
  if (enc)
  {
    enc->done();
  }
  return TRUE;
}

LASwritePoint::~LASwritePoint()
{
  U32 i;

  if (writers_raw)
  {
    for (i = 0; i < num_writers; i++)
    {
      delete writers_raw[i];
    }
    delete writers_raw;
  }
  if (writers_compressed)
  {
    for (i = 0; i < num_writers; i++)
    {
      delete writers_compressed[i];
    }
    delete writers_compressed;
  }
  if (enc)
  {
    delete enc;
  }
}
