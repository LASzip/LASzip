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

#if _MSC_VER < 1300
#include <fstream.h>
#else
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
/* returns how many bytes were written since last reset      */
  unsigned int byteCount() const;
/* reset byte counter                                        */
  void resetCount();
/* destructor                                                */
  ~ByteStreamOutOstream(){};
private:
  ostream* stream;
#if _MSC_VER < 1300
  long start;
#else
  ios::off_type start;
#endif
};

inline ByteStreamOutOstream::ByteStreamOutOstream(ostream* stream)
{
  this->stream = stream;
  resetCount();
}

inline bool ByteStreamOutOstream::putByte(unsigned char byte)
{
  stream->put(byte);
  return !!(stream->good());
}

inline bool ByteStreamOutOstream::putBytes(unsigned char* bytes, unsigned int num_bytes)
{
  stream->write( (char*) bytes, num_bytes);
  return !!(stream->good());
}

inline unsigned int ByteStreamOutOstream::byteCount() const
{
#if _MSC_VER < 1300
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

#endif
