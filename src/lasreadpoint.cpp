/*
===============================================================================

  FILE:  lasreadpoint.cpp
  
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

#include "lasreadpoint.hpp"

#include "arithmeticdecoder.hpp"
#include "lasreaditemraw.hpp"
#include "lasreaditemcompressed_v1.hpp"
#include "lasreaditemcompressed_v2.hpp"

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

BOOL LASreadPoint::setup(U32 num_items, const LASitem* items, LASzip::Algorithm algorithm)
{
  U32 i;

  // check if we support the items
  for (i = 0; i < num_items; i++)
  {
    if (!items[i].supported()) return FALSE;
  }

  // create entropy decoder (if requested)
  switch (algorithm)
  {
  case LASzip::POINT_BY_POINT_RAW:
    dec = 0;
    break;
  case LASzip::POINT_BY_POINT_ARITHMETIC:
  case LASzip::POINT_BY_POINT_ARITHMETIC_V2: // temporary fix
    dec = new ArithmeticDecoder();
    break;
  default:
    // entropy decoder not supported
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
        readers_raw[i] = new LASreadItemRaw_POINT10_LE();
      else
        readers_raw[i] = new LASreadItemRaw_POINT10_BE();
      break;
    case LASitem::GPSTIME11:
      if (IS_LITTLE_ENDIAN())
        readers_raw[i] = new LASreadItemRaw_GPSTIME11_LE();
      else
        readers_raw[i] = new LASreadItemRaw_GPSTIME11_BE();
        break;
    case LASitem::RGB12:
      if (IS_LITTLE_ENDIAN())
        readers_raw[i] = new LASreadItemRaw_RGB12_LE();
      else
        readers_raw[i] = new LASreadItemRaw_RGB12_BE();
      break;
    case LASitem::WAVEPACKET13:
      if (IS_LITTLE_ENDIAN())
        readers_raw[i] = new LASreadItemRaw_WAVEPACKET13_LE();
      else
        readers_raw[i] = new LASreadItemRaw_WAVEPACKET13_BE();
      break;
    case LASitem::BYTE:
      readers_raw[i] = new LASreadItemRaw_BYTE(items[i].size);
      break;
    default:
      return FALSE;
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
        if (items[i].version == 1)
          readers_compressed[i] = new LASreadItemCompressed_POINT10_v1(dec);
        else if (items[i].version == 2)
          readers_compressed[i] = new LASreadItemCompressed_POINT10_v2(dec);
        else
          return FALSE;
        break;
      case LASitem::GPSTIME11:
        if (items[i].version == 1)
          readers_compressed[i] = new LASreadItemCompressed_GPSTIME11_v1(dec);
        else if (items[i].version == 2)
          readers_compressed[i] = new LASreadItemCompressed_GPSTIME11_v2(dec);
        else
          return FALSE;
        break;
      case LASitem::RGB12:
        if (items[i].version == 1)
          readers_compressed[i] = new LASreadItemCompressed_RGB12_v1(dec);
        else if (items[i].version == 2)
          readers_compressed[i] = new LASreadItemCompressed_RGB12_v2(dec);
        else
          return FALSE;
        break;
      case LASitem::WAVEPACKET13:
        if (items[i].version == 1)
          readers_compressed[i] = new LASreadItemCompressed_WAVEPACKET13_v1(dec);
        else
          return FALSE;
        break;
      case LASitem::BYTE:
        if (items[i].version == 1)
          readers_compressed[i] = new LASreadItemCompressed_BYTE_v1(dec, items[i].size);
        else if (items[i].version == 2)
          readers_compressed[i] = new LASreadItemCompressed_BYTE_v2(dec, items[i].size);
        else
          return FALSE;
        break;
      default:
        return FALSE;
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

BOOL LASreadPoint::read(U8* const * point)
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

BOOL LASreadPoint::done()
{
  if (readers == readers_compressed)
  {
    dec->done();
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
    delete [] readers_raw;
  }

  if (readers_compressed)
  {
    for (i = 0; i < num_readers; i++)
    {
      delete readers_compressed[i];
    }
    delete [] readers_compressed;
  }

  if (dec)
  {
    delete dec;
  }
}
