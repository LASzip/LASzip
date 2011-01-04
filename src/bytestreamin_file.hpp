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

  FILE:  bytestreamin_file.h
  
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
#ifndef BYTE_STREAM_IN_FILE_H
#define BYTE_STREAM_IN_FILE_H

#include "bytestreamin.hpp"

#include <stdio.h>

class ByteStreamInFile : public ByteStreamIn
{
public:
  ByteStreamInFile(FILE* file);
/* read a single byte                                        */
  unsigned int getByte();
/* read an array of bytes                                    */
  bool getBytes(unsigned char* bytes, unsigned int num_bytes);
/* is the stream seekable (e.g. standard in is not)          */
  bool isSeekable() const;
/* save position in the stream for (forward) seeking later   */
  bool saveSeekPosition();
/* seek by offset from saved position (or start of file)     */
  bool seek(long offset);
/* seek to the end of the file                               */
  bool seekEnd();
/* returns how many bytes were read since last reset         */
  unsigned int byteCount() const;
/* reset byte counter                                        */
  void resetCount();
/* destructor                                                */
  ~ByteStreamInFile(){};
protected:
  FILE* file;
private:
  long start;
  long seek_position;
};

class ByteStreamInFileLE : public ByteStreamInFile
{
public:
  ByteStreamInFileLE(FILE* file);
/* read 16 bit low-endian field                              */
  bool get16bitsLE(unsigned char* bytes);
/* read 32 bit low-endian field                              */
  bool get32bitsLE(unsigned char* bytes);
/* read 64 bit low-endian field                              */
  bool get64bitsLE(unsigned char* bytes);
/* read 16 bit big-endian field                              */
  bool get16bitsBE(unsigned char* bytes);
/* read 32 bit big-endian field                              */
  bool get32bitsBE(unsigned char* bytes);
/* read 64 bit big-endian field                              */
  bool get64bitsBE(unsigned char* bytes);
private:
  unsigned char swapped[8];
};

class ByteStreamInFileBE : public ByteStreamInFile
{
public:
  ByteStreamInFileBE(FILE* file);
/* read 16 bit low-endian field                              */
  bool get16bitsLE(unsigned char* bytes);
/* read 32 bit low-endian field                              */
  bool get32bitsLE(unsigned char* bytes);
/* read 64 bit low-endian field                              */
  bool get64bitsLE(unsigned char* bytes);
/* read 16 bit big-endian field                              */
  bool get16bitsBE(unsigned char* bytes);
/* read 32 bit big-endian field                              */
  bool get32bitsBE(unsigned char* bytes);
/* read 64 bit big-endian field                              */
  bool get64bitsBE(unsigned char* bytes);
private:
  unsigned char swapped[8];
};

inline ByteStreamInFile::ByteStreamInFile(FILE* file)
{
  this->file = file;
  start = ftell(file);
  seek_position = ftell(file);
}

inline unsigned int ByteStreamInFile::getByte()
{
  int byte = getc(file);
  if (byte == EOF)
  {
//    fprintf(stderr, "reading EOF\n");
    byte = 0;
  }
  return (unsigned int)byte;
}

inline bool ByteStreamInFile::getBytes(unsigned char* bytes, unsigned int num_bytes)
{
  return (fread(bytes, 1, num_bytes, file) == num_bytes);
}

inline bool ByteStreamInFile::isSeekable() const
{
  return (file != stdin);
}

inline bool ByteStreamInFile::saveSeekPosition()
{
  seek_position = ftell(file);
  return (seek_position != -1L);
}

inline bool ByteStreamInFile::seek(long offset)
{
  return !fseek(file, seek_position+offset, SEEK_SET);
}

inline unsigned int ByteStreamInFile::byteCount() const
{
  return ftell(file)-start;
}

inline void ByteStreamInFile::resetCount()
{
  start = ftell(file);
}

inline ByteStreamInFileLE::ByteStreamInFileLE(FILE* file) : ByteStreamInFile(file)
{
}

inline bool ByteStreamInFileLE::get16bitsLE(unsigned char* bytes)
{
  return getBytes(bytes, 2);
}

inline bool ByteStreamInFileLE::get32bitsLE(unsigned char* bytes)
{
  return getBytes(bytes, 4);
}

inline bool ByteStreamInFileLE::get64bitsLE(unsigned char* bytes)
{
  return getBytes(bytes, 8);
}

inline bool ByteStreamInFileLE::get16bitsBE(unsigned char* bytes)
{
  if (getBytes(swapped, 2))
  {
    bytes[0] = swapped[1];
    bytes[1] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInFileLE::get32bitsBE(unsigned char* bytes)
{
  if (getBytes(swapped, 4))
  {
    bytes[0] = swapped[3];
    bytes[1] = swapped[2];
    bytes[2] = swapped[1];
    bytes[3] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInFileLE::get64bitsBE(unsigned char* bytes)
{
  if (getBytes(swapped, 8))
  {
    bytes[0] = swapped[7];
    bytes[1] = swapped[6];
    bytes[2] = swapped[5];
    bytes[3] = swapped[4];
    bytes[4] = swapped[3];
    bytes[5] = swapped[2];
    bytes[6] = swapped[1];
    bytes[7] = swapped[0];
    return true;
  }
  return false;
}

inline ByteStreamInFileBE::ByteStreamInFileBE(FILE* file) : ByteStreamInFile(file)
{
}

inline bool ByteStreamInFileBE::get16bitsLE(unsigned char* bytes)
{
  if (getBytes(swapped, 2))
  {
    bytes[0] = swapped[1];
    bytes[1] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInFileBE::get32bitsLE(unsigned char* bytes)
{
  if (getBytes(swapped, 4))
  {
    bytes[0] = swapped[3];
    bytes[1] = swapped[2];
    bytes[2] = swapped[1];
    bytes[3] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInFileBE::get64bitsLE(unsigned char* bytes)
{
  if (getBytes(swapped, 8))
  {
    bytes[0] = swapped[7];
    bytes[1] = swapped[6];
    bytes[2] = swapped[5];
    bytes[3] = swapped[4];
    bytes[4] = swapped[3];
    bytes[5] = swapped[2];
    bytes[6] = swapped[1];
    bytes[7] = swapped[0];
    return true;
  }
  return false;
}

inline bool ByteStreamInFileBE::get16bitsBE(unsigned char* bytes)
{
  return getBytes(bytes, 2);
}

inline bool ByteStreamInFileBE::get32bitsBE(unsigned char* bytes)
{
  return getBytes(bytes, 4);
}

inline bool ByteStreamInFileBE::get64bitsBE(unsigned char* bytes)
{
  return getBytes(bytes, 8);
}

#endif
