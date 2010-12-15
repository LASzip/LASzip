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

#if defined(_MSC_VER)
#if _MSC_VER < 1300
    #include <fstream.h>
#else
    #include <istream>
    #include <fstream>

#endif
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
/* returns how many bytes were read since last reset         */
  unsigned int byteCount() const;
/* reset byte counter                                        */
  void resetCount();
/* destructor                                                */
  ~ByteStreamInIstream(){};
private:
  istream* stream;
 

#if defined(_MSC_VER)
#if _MSC_VER < 1300
    long start;
#else
    ios::off_type start;
#endif
#else
    ios::off_type start;
#endif


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

  // http://stackoverflow.com/questions/604431/c-reading-unsigned-char-from-file-stream
  // WARNING, unsafe cast!!! -- hobu  

  stream->read( (char*) bytes, num_bytes);
  return !!(stream->good());
}

inline unsigned int ByteStreamInIstream::byteCount() const
{

#if defined(_MSC_VER)
#if _MSC_VER < 1300
    return (stream->tellg() - start);
#else
  ios::off_type size = stream->tellg() - start;
  return static_cast<unsigned int>(size);

#endif
#else
  ios::off_type size = stream->tellg() - start;
  return static_cast<unsigned int>(size);
#endif

}

inline void ByteStreamInIstream::resetCount()
{
  start = stream->tellg();
}

#endif
