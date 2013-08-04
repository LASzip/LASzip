/*
===============================================================================

  FILE:  bytestreamout.hpp
  
  CONTENTS:
      
    Abstract base class for output streams with endian handling.

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2007-2012, martin isenburg, rapidlasso - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
     1 October 2011 -- added 64 bit file support in MSVC 6.0 at McCafe at Hbf Linz
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_HPP
#define BYTE_STREAM_OUT_HPP

#include "mydefs.hpp"

class ByteStreamOut
{
public:
/* write a single byte                                       */
  virtual BOOL putByte(U8 byte) = 0;
/* write an array of bytes                                   */
  virtual BOOL putBytes(const U8* bytes, U32 num_bytes) = 0;
/* write 16 bit low-endian field                             */
  virtual BOOL put16bitsLE(const U8* bytes) = 0;
/* write 32 bit low-endian field                             */
  virtual BOOL put32bitsLE(const U8* bytes) = 0;
/* write 64 bit low-endian field                             */
  virtual BOOL put64bitsLE(const U8* bytes) = 0;
/* write 16 bit big-endian field                             */
  virtual BOOL put16bitsBE(const U8* bytes) = 0;
/* write 32 bit big-endian field                             */
  virtual BOOL put32bitsBE(const U8* bytes) = 0;
/* write 64 bit big-endian field                             */
  virtual BOOL put64bitsBE(const U8* bytes) = 0;
/* is the stream seekable (e.g. standard out is not)         */
  virtual BOOL isSeekable() const = 0;
/* get current position of stream                            */
  virtual I64 tell() const = 0;
/* seek to this position in the stream                       */
  virtual BOOL seek(const I64 position) = 0;
/* seek to the end of the file                               */
  virtual BOOL seekEnd() = 0;
/* destructor                                                */
  virtual ~ByteStreamOut() {};
};

#endif
