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

#include <istream.h>

class ByteStreamInIstream : public ByteStreamIn
{
public:
  ByteStreamInIstream(istream* stream);
/* read a single byte                                        */
  unsigned int getByte();
/* read an array of bytes                                    */
  bool getBytes(unsigned char* bytes, unsigned int num_bytes);
/* destructor                                                */
  ~ByteStreamInIstream(){};
private:
  istream* stream;
};

ByteStreamInIstream::ByteStreamInIstream(istream* stream)
{
  this->stream = stream;
}

unsigned int ByteStreamInIstream::getByte()
{
  int byte = stream->get();
  if (stream->bad())
  {
    byte = 0;
  }
  return (unsigned int)byte;
}

bool ByteStreamInIstream::getBytes(unsigned char* bytes, unsigned int num_bytes)
{
  stream->read(bytes, num_bytes);
  return !!(stream->good());
}

#endif
