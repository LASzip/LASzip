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
  point_size = 0;
  instream = 0;
  num_readers = 0;
  readers = 0;
  readers_raw = 0;
  readers_compressed = 0;
  dec = 0;
  // used for chunking
  chunk_size = U32_MAX;
  chunk_count = 0;
  current_chunk = 0;
  number_chunks = 0;
  chunk_totals = 0;
  chunk_starts = 0;
  // used for seeking
  seek_point = 0;
}

BOOL LASreadPoint::setup(U32 num_items, const LASitem* items, const LASzip* laszip)
{
  U32 i;

  // is laszip exists then we must use its items
  if (laszip)
  {
    if (num_items != laszip->num_items) return FALSE;
    if (items != laszip->items) return FALSE;
  }

  // create entropy decoder (if requested)
  dec = 0;
  if (laszip && laszip->compressor)
  {
    switch (laszip->coder)
    {
    case LASZIP_CODER_ARITHMETIC:
      dec = new ArithmeticDecoder();
      break;
    default:
      // entropy decoder not supported
      return FALSE;
    }
  }
 
  // initizalize the readers
  readers = 0;
  num_readers = num_items;

  // disable chunking
  chunk_size = U32_MAX;

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
    point_size += items[i].size;
  }

  if (dec)
  {
    readers_compressed = new LASreadItem*[num_readers];
    // seeks with compressed data need a seek point
    if (seek_point)
    {
      delete [] seek_point[0];
      delete [] seek_point;
    }
    seek_point = new U8*[num_items];
    if (!seek_point) return FALSE;
    seek_point[0] = new U8[point_size];
    if (!seek_point[0]) return FALSE;
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
      if (i) seek_point[i] = seek_point[i-1]+items[i-1].size;
    }
    if (laszip->compressor == LASZIP_COMPRESSOR_POINTWISE_CHUNKED)
    {
      if (laszip->chunk_size) chunk_size = laszip->chunk_size;
      number_chunks = U32_MAX;
    }
  }
  return TRUE;
}

BOOL LASreadPoint::init(ByteStreamIn* instream)
{
  if (!instream) return FALSE;
  this->instream = instream;

  // on very first init with chunking enabled
  if (number_chunks == U32_MAX)
  {
    if (!read_chunk_table())
    {
      return FALSE;
    }
    current_chunk = 0;
    if (chunk_totals) chunk_size = chunk_totals[1];
  }

  point_start = instream->tell();

  U32 i;
  for (i = 0; i < num_readers; i++)
  {
    ((LASreadItemRaw*)(readers_raw[i]))->init(instream);
  }

  if (dec)
  {
    readers = 0;
  }
  else
  {
    readers = readers_raw;
  }

  return TRUE;
}

BOOL LASreadPoint::seek(const U32 current, const U32 target)
{
  if (!instream->isSeekable()) return FALSE;
  U32 delta = 0;
  if (dec)
  {
    if (chunk_starts)
    {
      U32 target_chunk;
      if (chunk_totals)
      {
        target_chunk = search_chunk_table(target, 0, number_chunks);
        chunk_size = chunk_totals[target_chunk+1]-chunk_totals[target_chunk];
        delta = target - chunk_totals[target_chunk];
      }
      else
      {
        target_chunk = target/chunk_size;
        delta = target%chunk_size;
      }
      if (current_chunk != target_chunk || current > target)
      {
        dec->done();
        current_chunk = target_chunk;
        instream->seek(chunk_starts[current_chunk]);
        init(instream);
        chunk_count = 0;
      }
      else
      {
        delta = target - current;
      }
    }
    else if (current > target)
    {
      dec->done();
      instream->seek(point_start);
      init(instream);
      delta = target;
    }
    else if (current < target)
    {
      delta = target - current;
    }
    while (delta)
    {
      read(seek_point);
      delta--;
    }
  }
  else
  {
    if (current != target)
    {
      instream->seek(point_start+point_size*target);
    }
  }
  return TRUE;
}

BOOL LASreadPoint::read(U8* const * point)
{
  U32 i;

  try
  {
    if (dec)
    {
      if (chunk_count == chunk_size)
      {
        current_chunk++;
        if (chunk_totals)
        {
          chunk_size = chunk_totals[current_chunk+1]-chunk_totals[current_chunk];
        }
        dec->done();
        init(instream);
        chunk_count = 0;
      }
      chunk_count++;

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
    }
    else
    {
      for (i = 0; i < num_readers; i++)
      {
        readers[i]->read(point[i]);
      }
    }
  }
  catch (...)
  {
    return FALSE;
  }
  return TRUE;
}

BOOL LASreadPoint::done()
{
  if (readers == readers_compressed)
  {
    if (dec) dec->done();
  }
  return TRUE;
}

BOOL LASreadPoint::read_chunk_table()
{
  I64 chunks_start = instream->tell();
  I64 chunk_table_start_position;
  if (!instream->get64bitsLE((U8*)&chunk_table_start_position))
  {
    return FALSE;
  }
  if (chunk_table_start_position == chunks_start)
  {
    // compressor was interrupted and did not write chunk table 
    number_chunks = 0;
    return TRUE;
  }
  if (!instream->isSeekable())
  {
    // we cannot seek to the chunk table and back
    number_chunks = 0;
    return TRUE;
  }
  chunks_start += 8;
  if (chunk_table_start_position == -1)
  {
    // streaming compressor was wrote chunk table start at the end
    if (!instream->seekEnd(8))
    {
      return FALSE;
    }
    if (!instream->get64bitsLE((U8*)&chunk_table_start_position))
    {
      return FALSE;
    }
  }
  else
  {
    instream->seek(chunk_table_start_position);
    U32 dummy;
    if (!instream->get32bitsLE((U8*)&dummy))
    {
      // the file transfer was interrupted. no chunk table.
      if (!instream->seek(chunks_start))
      {
        return FALSE;
      }
      number_chunks = 0;
      return TRUE;
    }
  }
  if (!instream->seek(chunk_table_start_position))
  {
    return FALSE;
  }
  U32 version;
  if (!instream->get32bitsLE((U8*)&version) || version != 0)
  {
    return FALSE;
  }
  if (!instream->get32bitsLE((U8*)&number_chunks))
  {
    return FALSE;
  }
  if (chunk_totals) delete [] chunk_totals;
  chunk_totals = 0;
  if (chunk_starts) delete [] chunk_starts;
  chunk_starts = 0;
  if (chunk_size == U32_MAX) chunk_totals = new U32[number_chunks+1];
  chunk_starts = new I64[number_chunks+1];
  if (chunk_size == U32_MAX) chunk_totals[0] = 0;
  chunk_starts[0] = chunks_start;
  if (number_chunks > 0)
  {
    U32 i;
    dec->init(instream);
    IntegerCompressor ic(dec, 32, 2);
    ic.initDecompressor();
    for (i = 1; i <= number_chunks; i++)
    {
      if (chunk_size == U32_MAX) chunk_totals[i] = ic.decompress((i>1 ? chunk_totals[i-1] : 0), 0);
      chunk_starts[i] = ic.decompress((i>1 ? (U32)(chunk_starts[i-1]) : 0), 1);
    }
    dec->done();
    for (i = 1; i <= number_chunks; i++)
    {
      if (chunk_size == U32_MAX) chunk_totals[i] += chunk_totals[i-1];
      chunk_starts[i] += chunk_starts[i-1];
    }
  }
  if (!instream->seek(chunks_start))
  {
    return FALSE;
  }
  return TRUE;
}

U32 LASreadPoint::search_chunk_table(const U32 index, const U32 lower, const U32 upper)
{
  if (lower + 1 == upper) return lower;
  U32 mid = (lower+upper)/2;
  if (index >= chunk_totals[mid])
    return search_chunk_table(index, mid, upper);
  else
    return search_chunk_table(index, lower, mid);
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

  if (chunk_starts) delete [] chunk_starts;

  if (seek_point)
  {
    delete [] seek_point[0];
    delete [] seek_point;
  }
}
