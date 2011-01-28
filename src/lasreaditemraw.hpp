/*
===============================================================================

  FILE:  LASitemreadraw.hpp
  
  CONTENTS:
  
    Implementation of LASitemReadRaw for *all* items that compose a point.

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
    7 December 2010 -- refactored after getting invited to KAUST in Saudi Arabia
  
===============================================================================
*/
#ifndef LAS_READ_ITEM_RAW_H
#define LAS_READ_ITEM_RAW_H

#include "lasreaditem.hpp"

#include <assert.h>

class LASreadItemRaw_POINT10_LE : public LASreadItemRaw
{
public:
  LASreadItemRaw_POINT10_LE(){};
  inline BOOL read(U8* item)
  {
    return instream->getBytes(item, 20);
  }
};

class LASreadItemRaw_POINT10_BE : public LASreadItemRaw
{
public:
  LASreadItemRaw_POINT10_BE(){};
  inline BOOL read(U8* item)
  {
    if (!instream->getBytes(swapped, 20)) return FALSE;
    ENDIAN_SWAP_32(&swapped[ 0], &item[ 0]);    // x
    ENDIAN_SWAP_32(&swapped[ 4], &item[ 4]);    // y
    ENDIAN_SWAP_32(&swapped[ 8], &item[ 8]);    // z
    ENDIAN_SWAP_16(&swapped[12], &item[12]);    // intensity
    *((U32*)&item[14]) = *((U32*)&swapped[14]); // bitfield, classification, scan_angle_rank, user_data
    ENDIAN_SWAP_16(&swapped[18], &item[18]);    // point_source_ID
    return TRUE;
  };
private:
  U8 swapped[20];
};

class LASreadItemRaw_GPSTIME11_LE : public LASreadItemRaw
{
public:
  LASreadItemRaw_GPSTIME11_LE(){};
  inline BOOL read(U8* item)
  {
    return instream->getBytes(item, 8);
  };
};

class LASreadItemRaw_GPSTIME11_BE : public LASreadItemRaw
{
public:
  LASreadItemRaw_GPSTIME11_BE(){};
  inline BOOL read(U8* item)
  {
    if (!instream->getBytes(swapped, 8)) return FALSE;
    ENDIAN_SWAP_64(swapped, item);
    return TRUE;
  };
private:
  U8 swapped[8];
};

class LASreadItemRaw_RGB12_LE : public LASreadItemRaw
{
public:
  LASreadItemRaw_RGB12_LE(){};
  inline BOOL read(U8* item)
  {
    return instream->getBytes(item, 6);
  };
};

class LASreadItemRaw_RGB12_BE : public LASreadItemRaw
{
public:
  LASreadItemRaw_RGB12_BE(){};
  inline BOOL read(U8* item)
  {
    if (!instream->getBytes(swapped, 6)) return FALSE;
    ENDIAN_SWAP_32(&swapped[ 0], &item[ 0]); // R
    ENDIAN_SWAP_32(&swapped[ 2], &item[ 2]); // G
    ENDIAN_SWAP_32(&swapped[ 4], &item[ 4]); // B
    return TRUE;
  };
private:
  U8 swapped[6];
};

class LASreadItemRaw_WAVEPACKET13_LE : public LASreadItemRaw
{
public:
  LASreadItemRaw_WAVEPACKET13_LE(){}
  inline BOOL read(U8* item)
  {
    return instream->getBytes(item, 29);
  };
};

class LASreadItemRaw_WAVEPACKET13_BE : public LASreadItemRaw
{
public:
  LASreadItemRaw_WAVEPACKET13_BE(){}
  inline BOOL read(U8* item)
  {
    if (!instream->getBytes(swapped, 29)) return FALSE;
    item[0] = swapped[0];                    // wavepacket descriptor index
    ENDIAN_SWAP_64(&swapped[ 1], &item[ 1]); // byte offset to waveform data
    ENDIAN_SWAP_32(&swapped[ 9], &item[ 9]); // waveform packet size in bytes
    ENDIAN_SWAP_32(&swapped[13], &item[13]); // return point waveform location
    ENDIAN_SWAP_32(&swapped[17], &item[17]); // X(t)
    ENDIAN_SWAP_32(&swapped[21], &item[21]); // Y(t)
    ENDIAN_SWAP_32(&swapped[25], &item[25]); // Z(t)
    return TRUE;
  };
private:
  U8 swapped[29];
};

class LASreadItemRaw_BYTE : public LASreadItemRaw
{
public:
  LASreadItemRaw_BYTE(U32 number)
  {
    this->number = number;
  }
  inline BOOL read(U8* item)
  {
    return instream->getBytes(item, number);
  };
private:
  U32 number;
};

#endif
