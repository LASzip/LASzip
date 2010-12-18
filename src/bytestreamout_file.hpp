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

  FILE:  bytestreamout_file.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    8 December 2010 -- unified framework for all entropy coders
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_FILE_H
#define BYTE_STREAM_OUT_FILE_H

#include "bytestreamout.hpp"

#include <stdio.h>

class ByteStreamOutFile : public ByteStreamOut
{
public:
  ByteStreamOutFile(FILE* file);
/* write a single byte                                       */
  bool putByte(unsigned char byte);
/* write an array of bytes                                   */
  bool putBytes(const unsigned char* bytes, unsigned int num_bytes);
/* write 16 bit field (for implementing endian swap)         */
  virtual bool put16bits(const unsigned char* bytes);
/* write 32 bit field (for implementing endian swap)         */
  virtual bool put32bits(const unsigned char* bytes);
/* write 64 bit field (for implementing endian swap)         */
  virtual bool put64bits(const unsigned char* bytes);
/* is the stream seekable (e.g. standard out is not)         */
  bool isSeekable() const;
/* save position in the stream for (forward) seeking later   */
  bool saveSeekPosition();
/* seek forward from last saved position (or start of file)  */
  bool seek(long offset);
/* seek to the end of the file                               */
  bool seekEnd();
/* returns how many bytes were written                       */
  unsigned int byteCount() const;
/* returns how many bytes were written                       */
  void resetCount();
/* destructor                                                */
  ~ByteStreamOutFile(){};
protected:
  FILE* file;
private:
  long start;
  long seek_position;
};

class ByteStreamOutFileEndianSwapped : public ByteStreamOutFile
{
public:
  ByteStreamOutFileEndianSwapped(FILE* file);
/* write 16 bit field (for implementing endian swap)         */
  bool put16bits(const unsigned char* bytes);
/* write 32 bit field (for implementing endian swap)         */
  bool put32bits(const unsigned char* bytes);
/* write 64 bit field (for implementing endian swap)         */
  bool put64bits(const unsigned char* bytes);
private:
  unsigned char swapped[8];
};

inline ByteStreamOutFile::ByteStreamOutFile(FILE* file)
{
  this->file = file;
  start = ftell(file);
  seek_position = ftell(file);
}

inline bool ByteStreamOutFile::putByte(unsigned char byte)
{
  return (fputc(byte, file) == byte);
}

inline bool ByteStreamOutFile::putBytes(const unsigned char* bytes, unsigned int num_bytes)
{
  return (fwrite(bytes, 1, num_bytes, file) == num_bytes);
}

inline bool ByteStreamOutFile::put16bits(const unsigned char* bytes)
{
  return putBytes(bytes, 2);
}

inline bool ByteStreamOutFile::put32bits(const unsigned char* bytes)
{
  return putBytes(bytes, 4);
}

inline bool ByteStreamOutFile::put64bits(const unsigned char* bytes)
{
  return putBytes(bytes, 8);
}

inline bool ByteStreamOutFile::isSeekable() const
{
  return (file != stdout);
}

inline bool ByteStreamOutFile::saveSeekPosition()
{
  seek_position = ftell(file);
  return (seek_position != -1L);
}

inline bool ByteStreamOutFile::seek(long offset)
{
  return !(fseek(file, seek_position+offset, SEEK_SET));
}

inline bool ByteStreamOutFile::seekEnd()
{
  return !(fseek(file, 0, SEEK_END));
}

inline unsigned int ByteStreamOutFile::byteCount() const
{
  return ftell(file)-start;
}

inline void ByteStreamOutFile::resetCount()
{
  start = ftell(file);
}

inline ByteStreamOutFileEndianSwapped::ByteStreamOutFileEndianSwapped(FILE* file) : ByteStreamOutFile(file)
{
}

inline bool ByteStreamOutFileEndianSwapped::put16bits(const unsigned char* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline bool ByteStreamOutFileEndianSwapped::put32bits(const unsigned char* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline bool ByteStreamOutFileEndianSwapped::put64bits(const unsigned char* bytes)
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
