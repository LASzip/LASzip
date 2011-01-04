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

  FILE:  bytestreamin.hpp
  
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
#ifndef BYTE_STREAM_IN_H
#define BYTE_STREAM_IN_H

class ByteStreamIn
{
public:
/* read a single byte                                        */
  virtual unsigned int getByte() = 0;
/* read an array of bytes                                    */
  virtual bool getBytes(unsigned char* bytes, unsigned int num_bytes) = 0;
/* read 16 bit low-endian field                              */
  virtual bool get16bitsLE(unsigned char* bytes) = 0;
/* read 32 bit low-endian field                              */
  virtual bool get32bitsLE(unsigned char* bytes) = 0;
/* read 64 bit low-endian field                              */
  virtual bool get64bitsLE(unsigned char* bytes) = 0;
/* read 16 bit big-endian field                              */
  virtual bool get16bitsBE(unsigned char* bytes) = 0;
/* read 32 bit big-endian field                              */
  virtual bool get32bitsBE(unsigned char* bytes) = 0;
/* read 64 bit big-endian field                              */
  virtual bool get64bitsBE(unsigned char* bytes) = 0;
/* is the stream seekable (e.g. standard in is not)          */
  virtual bool isSeekable() const = 0;
/* returns how many bytes were read since last reset         */
  virtual unsigned int byteCount() const = 0;
/* reset byte counter                                        */
  virtual void resetCount() = 0;
/* destructor                                                */
  virtual ~ByteStreamIn() {};
};

#endif
