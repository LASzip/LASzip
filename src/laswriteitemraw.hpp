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
  
    7 January 2011 -- introduced swap buffers to reduce number of fwrite calls
    12 December 2010 -- refactored after watching two movies with silke
  
===============================================================================
*/
#ifndef LAS_WRITE_ITEM_RAW_H
#define LAS_WRITE_ITEM_RAW_H

#include "laswriteitem.hpp"

#include <assert.h>

class LASwriteItemRaw_POINT10_LE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_POINT10_LE(){};
  inline BOOL write(const U8* item)
  {
    return outstream->putBytes(item, 20);
  };
};

class LASwriteItemRaw_POINT10_BE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_POINT10_BE(){};
  inline BOOL write(const U8* item)
  {
    ENDIAN_SWAP_32(&item[ 0], &swapped[ 0]);    // x
    ENDIAN_SWAP_32(&item[ 4], &swapped[ 4]);    // y
    ENDIAN_SWAP_32(&item[ 8], &swapped[ 8]);    // z
    ENDIAN_SWAP_16(&item[12], &swapped[12]);    // intensity
    *((U32*)&swapped[14]) = *((U32*)&item[14]); // bitfield, classification, scan_angle_rank, user_data
    ENDIAN_SWAP_16(&item[18], &swapped[18]);    // point_source_ID
    return outstream->putBytes(swapped, 20);
  };
private:
  U8 swapped[20];
};

class LASwriteItemRaw_GPSTIME11_LE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_GPSTIME11_LE() {};
  inline BOOL write(const U8* item)
  {
    return outstream->putBytes(item, 8);
  };
};

class LASwriteItemRaw_GPSTIME11_BE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_GPSTIME11_BE() {};
  inline BOOL write(const U8* item)
  {
    ENDIAN_SWAP_64(item, swapped);
    return outstream->putBytes(swapped, 8);
  };
private:
  U8 swapped[8];
};

class LASwriteItemRaw_RGB12_LE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_RGB12_LE(){}
  inline BOOL write(const U8* item)
  {
    return outstream->putBytes(item, 6);
  };
};

class LASwriteItemRaw_RGB12_BE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_RGB12_BE(){}
  inline BOOL write(const U8* item)
  {
    ENDIAN_SWAP_32(&item[ 0], &swapped[ 0]); // R
    ENDIAN_SWAP_32(&item[ 2], &swapped[ 2]); // G
    ENDIAN_SWAP_32(&item[ 4], &swapped[ 4]); // B
    return outstream->putBytes(swapped, 6);
  };
private:
  U8 swapped[6];
};

class LASwriteItemRaw_WAVEPACKET13_LE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_WAVEPACKET13_LE(){}
  inline BOOL write(const U8* item)
  {
    return outstream->putBytes(item, 29);
  };
};

class LASwriteItemRaw_WAVEPACKET13_BE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_WAVEPACKET13_BE(){}
  inline BOOL write(const U8* item)
  {
    swapped[0] = item[0];                    // wavepacket descriptor index
    ENDIAN_SWAP_64(&item[ 1], &swapped[ 1]); // byte offset to waveform data
    ENDIAN_SWAP_32(&item[ 9], &swapped[ 9]); // waveform packet size in bytes
    ENDIAN_SWAP_32(&item[13], &swapped[13]); // return point waveform location
    ENDIAN_SWAP_32(&item[17], &swapped[17]); // X(t)
    ENDIAN_SWAP_32(&item[21], &swapped[21]); // Y(t)
    ENDIAN_SWAP_32(&item[25], &swapped[25]); // Z(t)
    return outstream->putBytes(swapped, 29);
  };
private:
  U8 swapped[29];
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
