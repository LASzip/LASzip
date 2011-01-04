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

  FILE:  LASwriteitemraw.hpp
  
  CONTENTS:
  
    Implementation of LASwriteItemRaw for *all* items that compose a point.

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    12 December 2010 -- refactored after watching two movies with silke
  
===============================================================================
*/
#ifndef LAS_WRITE_ITEM_RAW_H
#define LAS_WRITE_ITEM_RAW_H

#include "laswriteitem.hpp"

#include <assert.h>

class LASwriteItemRaw_POINT10 : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_POINT10(){};
  inline BOOL write(const U8* item)
  {
    if (!outstream->put32bitsLE(&(item[ 0]))) return FALSE;       // x
    if (!outstream->put32bitsLE(&(item[ 4]))) return FALSE;       // y
    if (!outstream->put32bitsLE(&(item[ 8]))) return FALSE;       // z
    if (!outstream->put16bitsLE(&(item[12]))) return FALSE;       // intensity
    if (!outstream->putBytes(&(item[14]), 4)) return FALSE;       // bitfield
                                                                  // classification
                                                                  // scan_angle_rank
                                                                  // user_data
    if (!outstream->put16bitsLE(&(item[18]))) return FALSE;       // point_source_ID
    return TRUE;
  };
};

class LASwriteItemRaw_GPSTIME11 : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_GPSTIME11() {};
  inline BOOL write(const U8* item)
  {
    return outstream->put64bitsLE(item);                          // GPSTIME
  };
};

class LASwriteItemRaw_RGB12 : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_RGB12(){}
  inline BOOL write(const U8* item)
  {
    if (!outstream->put16bitsLE(&(item[0]))) return FALSE;        // R
    if (!outstream->put16bitsLE(&(item[2]))) return FALSE;        // G
    if (!outstream->put16bitsLE(&(item[4]))) return FALSE;        // B
    return TRUE;
  };
};

class LASwriteItemRaw_WAVEPACKET13 : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_WAVEPACKET13(){}
  inline BOOL write(const U8* item)
  {
    if (!outstream->putBytes(&(item[0]),1)) return FALSE;         // wavepacket descriptor index
    if (!outstream->put64bitsLE(&(item[ 1]))) return FALSE;       // byte offset to waveform data
    if (!outstream->put32bitsLE(&(item[ 9]))) return FALSE;       // waveform packet size in bytes
    if (!outstream->put32bitsLE(&(item[13]))) return FALSE;       // return point waveform location
    if (!outstream->put32bitsLE(&(item[17]))) return FALSE;       // X(t)
    if (!outstream->put32bitsLE(&(item[21]))) return FALSE;       // Y(t)
    if (!outstream->put32bitsLE(&(item[25]))) return FALSE;       // Z(t)
    return TRUE;
  };
};

class LASwriteItemRaw_BYTE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_BYTE(U32 number)
  {
    this->number = number;
  }
  inline BOOL write(const U8* item)
  {
    return outstream->putBytes(item, number);
  };
private:
  U32 number;
};

#endif
