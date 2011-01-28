/*
===============================================================================

  FILE:  bytestreamout.hpp
  
  CONTENTS:
      
  PROGRAMMERS:
  
    martin.isenburg@gmail.com
  
  COPYRIGHT:

    (c) 2010-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_H
#define BYTE_STREAM_OUT_H

#include "mydefs.hpp"

class ByteStreamOut
{
public:
/* write a single byte                                       */
  virtual bool putByte(unsigned char byte) = 0;
/* write an array of bytes                                   */
  virtual bool putBytes(const unsigned char* bytes, unsigned int num_bytes) = 0;
/* write 16 bit low-endian field                             */
  virtual bool put16bitsLE(const unsigned char* bytes) = 0;
/* write 32 bit low-endian field                             */
  virtual bool put32bitsLE(const unsigned char* bytes) = 0;
/* write 64 bit low-endian field                             */
  virtual bool put64bitsLE(const unsigned char* bytes) = 0;
/* write 16 bit big-endian field                             */
  virtual bool put16bitsBE(const unsigned char* bytes) = 0;
/* write 32 bit big-endian field                             */
  virtual bool put32bitsBE(const unsigned char* bytes) = 0;
/* write 64 bit big-endian field                             */
  virtual bool put64bitsBE(const unsigned char* bytes) = 0;
/* is the stream seekable (e.g. standard out is not)         */
  virtual bool isSeekable() const = 0;
/* save position in the stream for (forward) seeking later   */
  virtual bool saveSeekPosition() = 0;
/* seek forward from last saved position (or start of file)  */
  virtual bool seek(long offset) = 0;
/* seek to the end of the file                               */
  virtual bool seekEnd() = 0;
/* returns how many bytes were written since reset           */
  virtual unsigned int byteCount() const = 0;
/* returns how many bytes were written                       */
  virtual void resetCount() = 0;
/* destructor                                                */
  virtual ~ByteStreamOut() {};
};

#endif
