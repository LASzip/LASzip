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

  FILE:  bytestreamout_ostream.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    12 December 2010 -- created from ByteStreamOutOstream after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_OSTREAM_H
#define BYTE_STREAM_OUT_OSTREAM_H

#include "bytestreamout.hpp"

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
using namespace std;
#endif

class ByteStreamOutOstream : public ByteStreamOut
{
public:
  ByteStreamOutOstream(ostream* stream);
/* write a single byte                                       */
  bool putByte(unsigned char byte);
/* write an array of bytes                                   */
  bool putBytes(unsigned char* bytes, unsigned int num_bytes);
/* write 16 bit field (for implementing endian swap)         */
  virtual bool put16bits(unsigned char* bytes);
/* write 32 bit field (for implementing endian swap)         */
  virtual bool put32bits(unsigned char* bytes);
/* write 64 bit field (for implementing endian swap)         */
  virtual bool put64bits(unsigned char* bytes);
/* is the stream seekable (e.g. standard out is not)         */
  bool isSeekable() const;
/* save position in the stream for (forward) seeking later   */
  bool saveSeekPosition();
/* seek by offset from saved position (or start of file)     */
  bool seek(long offset);
/* seek to the end of the file                               */
  bool seekEnd();
/* returns how many bytes were written since last reset      */
  unsigned int byteCount() const;
/* reset byte counter                                        */
  void resetCount();
/* destructor                                                */
  ~ByteStreamOutOstream(){};
protected:
  ostream* stream;
private:
#ifdef LZ_WIN32_VC6
  long start;
  long seek_position;
#else
  ios::off_type start;
  ios::off_type seek_position;
#endif
};

class ByteStreamOutOstreamEndianSwapped : public ByteStreamOutOstream
{
public:
  ByteStreamOutOstreamEndianSwapped(ostream* stream);
/* write 16 bit field (for implementing endian swap)         */
  bool put16bits(unsigned char* bytes);
/* write 32 bit field (for implementing endian swap)         */
  bool put32bits(unsigned char* bytes);
/* write 64 bit field (for implementing endian swap)         */
  bool put64bits(unsigned char* bytes);
private:
  unsigned char swapped[8];
};

inline ByteStreamOutOstream::ByteStreamOutOstream(ostream* stream)
{
  this->stream = stream;
  start = stream->tellp();
  seek_position = stream->tellp();
}

inline bool ByteStreamOutOstream::putByte(unsigned char byte)
{
  stream->put(byte);
  return !!(stream->good());
}

inline bool ByteStreamOutOstream::putBytes(unsigned char* bytes, unsigned int num_bytes)
{
  stream->write((const char*)bytes, num_bytes);
  return !!(stream->good());
}

inline bool ByteStreamOutOstream::put16bits(unsigned char* bytes)
{
  return putBytes(bytes, 2);
}

inline bool ByteStreamOutOstream::put32bits(unsigned char* bytes)
{
  return putBytes(bytes, 4);
}

inline bool ByteStreamOutOstream::put64bits(unsigned char* bytes)
{
  return putBytes(bytes, 8);
}

inline bool ByteStreamOutOstream::isSeekable() const
{
  return (!!(static_cast<ofstream&>(*stream)));
}

inline bool ByteStreamOutOstream::saveSeekPosition()
{
  seek_position = stream->tellp();
  return !!(stream->good());
}

inline bool ByteStreamOutOstream::seek(long offset)
{
  stream->seekp(seek_position+offset);
  return !!(stream->good());
}

inline bool ByteStreamOutOstream::seekEnd()
{
  stream->seekp(0, ios::end);
  return !!(stream->good());
}

inline unsigned int ByteStreamOutOstream::byteCount() const
{
#ifdef LZ_WIN32_VC6
  return (stream->tellp() - start);
#else
  ios::pos_type end = stream->tellp();
  ios::off_type size = end - start;
  return static_cast<unsigned int>(size);
#endif
}

inline void ByteStreamOutOstream::resetCount()
{
  start = stream->tellp();
}

inline ByteStreamOutOstreamEndianSwapped::ByteStreamOutOstreamEndianSwapped(ostream* stream) : ByteStreamOutOstream(stream)
{
}

inline bool ByteStreamOutOstreamEndianSwapped::put16bits(unsigned char* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline bool ByteStreamOutOstreamEndianSwapped::put32bits(unsigned char* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline bool ByteStreamOutOstreamEndianSwapped::put64bits(unsigned char* bytes)
{
  swapped[0] = bytes[7];
  swapped[1] = bytes[6];
  swapped[2] = bytes[5];
  swapped[3] = bytes[4];
  swapped[4] = bytes[3];
  swapped[5] = bytes[2];
  swapped[6] = bytes[1];
  swapped[7] = bytes[0];
  return putBytes(swapped, 8);
}

#endif
