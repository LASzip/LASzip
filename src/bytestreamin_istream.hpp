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

  FILE:  bytestreamin_istream.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_IN_ISTREAM_H
#define BYTE_STREAM_IN_ISTREAM_H

#include "bytestreamin.hpp"

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
using namespace std;
#endif

class ByteStreamInIstream : public ByteStreamIn
{
public:
  ByteStreamInIstream(istream* stream);
/* read a single byte                                        */
  unsigned int getByte();
/* read an array of bytes                                    */
  bool getBytes(unsigned char* bytes, unsigned int num_bytes);
/* read 16 bit field (for implementing endian swap)         */
  virtual bool get16bits(unsigned char* bytes);
/* read 32 bit field (for implementing endian swap)         */
  virtual bool get32bits(unsigned char* bytes);
/* read 64 bit field (for implementing endian swap)         */
  virtual bool get64bits(unsigned char* bytes);
/* is the stream seekable (e.g. standard in is not)          */
  bool isSeekable() const;
/* returns how many bytes were read since last reset         */
  unsigned int byteCount() const;
/* reset byte counter                                        */
  void resetCount();
/* destructor                                                */
  ~ByteStreamInIstream(){};
protected:
  istream* stream;
private:
#ifdef LZ_WIN32_VC6
  long start;
#else
  ios::off_type start;
#endif
};

class ByteStreamInIstreamEndianSwapped : public ByteStreamInIstream
{
public:
  ByteStreamInIstreamEndianSwapped(istream* stream);
/* read 16 bit field (for implementing endian swap)         */
  bool get16bits(unsigned char* bytes);
/* read 32 bit field (for implementing endian swap)         */
  bool get32bits(unsigned char* bytes);
/* read 64 bit field (for implementing endian swap)         */
  bool get64bits(unsigned char* bytes);
private:
  unsigned char swapped[8];
};

inline ByteStreamInIstream::ByteStreamInIstream(istream* stream)
{
  this->stream = stream;
  resetCount();
}

inline unsigned int ByteStreamInIstream::getByte()
{
  int byte = stream->get();
  if (stream->eof())
  {
    fprintf(stderr, "reading EOF\n");
    byte = 0;
  }
  return (unsigned int)byte;
}

inline bool ByteStreamInIstream::getBytes(unsigned char* bytes, unsigned int num_bytes)
{
  stream->read((char*)bytes, num_bytes);
  return !!(stream->good());
}

inline bool ByteStreamInIstream::get16bits(unsigned char* bytes)
{
  stream->read((char*)bytes, 2);
  return !!(stream->good());
}

inline bool ByteStreamInIstream::get32bits(unsigned char* bytes)
{
  stream->read((char*)bytes, 4);
  return !!(stream->good());
}

inline bool ByteStreamInIstream::get64bits(unsigned char* bytes)
{
  stream->read((char*)bytes, 8);
  return !!(stream->good());
}

inline bool ByteStreamInIstream::isSeekable() const
{
  return (!!(static_cast<ifstream&>(*stream)));
}

inline unsigned int ByteStreamInIstream::byteCount() const
{
#ifdef LZ_WIN32_VC6
  return (stream->tellg() - start);
#else
  ios::pos_type end = stream->tellg();
  ios::off_type size = end - start;
  return static_cast<unsigned int>(size);
#endif
}

inline void ByteStreamInIstream::resetCount()
{
  start = stream->tellg();
}

inline ByteStreamInIstreamEndianSwapped::ByteStreamInIstreamEndianSwapped(istream* stream) : ByteStreamInIstream(stream)
{
}

inline bool ByteStreamInIstreamEndianSwapped::get16bits(unsigned char* bytes)
{
  stream->read((char*)swapped, 2);
  bytes[0] = swapped[1];
  bytes[1] = swapped[0];
  return !!(stream->good());
}

inline bool ByteStreamInIstreamEndianSwapped::get32bits(unsigned char* bytes)
{
  stream->read((char*)swapped, 4);
  bytes[0] = swapped[3];
  bytes[1] = swapped[2];
  bytes[2] = swapped[1];
  bytes[3] = swapped[0];
  return !!(stream->good());
}

inline bool ByteStreamInIstreamEndianSwapped::get64bits(unsigned char* bytes)
{
  stream->read((char*)swapped, 8);
  bytes[0] = swapped[7];
  bytes[1] = swapped[6];
  bytes[2] = swapped[5];
  bytes[3] = swapped[4];
  bytes[4] = swapped[3];
  bytes[5] = swapped[2];
  bytes[6] = swapped[1];
  bytes[7] = swapped[0];
  return !!(stream->good());
}

#endif
