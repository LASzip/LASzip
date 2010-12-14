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
/* destructor                                                */
  ~ByteStreamInFile(){};
private:
  FILE* fp;
};

ByteStreamInFile::ByteStreamInFile(FILE* file)
{
  fp = file;
}

unsigned int ByteStreamInFile::getByte()
{
  int byte = getc(fp);
  if (byte == EOF)
  {
    byte = 0;
  }
  return (unsigned int)byte;
}

bool ByteStreamInFile::getBytes(unsigned char* bytes, unsigned int num_bytes)
{
  return (fread(bytes, 1, num_bytes, fp) != num_bytes);
}

#endif
