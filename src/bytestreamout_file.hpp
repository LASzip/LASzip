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
  bool putBytes(unsigned char* bytes, unsigned int num_bytes);
/* returns how many bytes were written                       */
  unsigned int byteCount() const;
/* returns how many bytes were written                       */
  void resetCount();
/* destructor                                                */
  ~ByteStreamOutFile(){};
private:
  FILE* file;
  long start;
};

ByteStreamOutFile::ByteStreamOutFile(FILE* file)
{
  this->file = file;
  resetCount();
}

bool ByteStreamOutFile::putByte(unsigned char byte)
{
  return (fputc(byte, file) == byte);
}

bool ByteStreamOutFile::putBytes(unsigned char* bytes, unsigned int num_bytes)
{
  return (fwrite(bytes, 1, num_bytes, file) == num_bytes);
}

unsigned int ByteStreamOutFile::byteCount() const
{
  return ftell(file)-start;
}

void ByteStreamOutFile::resetCount()
{
  start = ftell(file);
}

#endif
