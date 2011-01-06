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

  FILE:  LASitemreadraw.hpp
  
  CONTENTS:
  
    Implementation of LASitemReadRaw for *all* items that compose a point.

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 December 2010 -- refactored after getting invited to KAUST in Saudi Arabia
  
===============================================================================
*/
#ifndef LAS_READ_ITEM_RAW_H
#define LAS_READ_ITEM_RAW_H

#include "lasreaditem.hpp"

#include <assert.h>

class LASreadItemRaw_POINT10 : public LASreadItemRaw
{
public:
  LASreadItemRaw_POINT10(){};
  inline BOOL read(U8* item)
  {
    if (!instream->getBytes(item, 20)) return FALSE;       // POINT10
    return TRUE;
  }
};

/*
class LASreadItemRaw_POINT10 : public LASreadItemRaw
{
public:
  LASreadItemRaw_POINT10(){};
  inline BOOL read(U8* item)
  {
    if (!instream->get32bitsLE(&(item[ 0]))) return FALSE;       // x
    if (!instream->get32bitsLE(&(item[ 4]))) return FALSE;       // y
    if (!instream->get32bitsLE(&(item[ 8]))) return FALSE;       // z
    if (!instream->get16bitsLE(&(item[12]))) return FALSE;       // intensity
    if (!instream->getBytes(&(item[14]), 4)) return FALSE;       // bitfield
                                                                 // classification
                                                                 // scan_angle_rank
                                                                 // user_data
    if (!instream->get16bitsLE(&(item[18]))) return FALSE;       // point_source_ID
    return TRUE;
  };
};
*/

class LASreadItemRaw_GPSTIME11 : public LASreadItemRaw
{
public:
  LASreadItemRaw_GPSTIME11(){};
  inline BOOL read(U8* item)
  {
    return instream->get64bitsLE(item);                          // GPSTIME
  };
};

class LASreadItemRaw_RGB12 : public LASreadItemRaw
{
public:
  LASreadItemRaw_RGB12(){};
  inline BOOL read(U8* item)
  {
    if (!instream->get16bitsLE(&(item[0]))) return FALSE;        // R
    if (!instream->get16bitsLE(&(item[2]))) return FALSE;        // G
    if (!instream->get16bitsLE(&(item[4]))) return FALSE;        // B
    return TRUE;
  };
};

class LASreadItemRaw_WAVEPACKET13 : public LASreadItemRaw
{
public:
  LASreadItemRaw_WAVEPACKET13(){}
  inline BOOL read(U8* item)
  {
    if (!instream->getBytes(&(item[0]),1)) return FALSE;         // wavepacket descriptor index
    if (!instream->get64bitsLE(&(item[ 1]))) return FALSE;       // byte offset to waveform data
    if (!instream->get32bitsLE(&(item[ 9]))) return FALSE;       // waveform packet size in bytes
    if (!instream->get32bitsLE(&(item[13]))) return FALSE;       // return point waveform location
    if (!instream->get32bitsLE(&(item[17]))) return FALSE;       // X(t)
    if (!instream->get32bitsLE(&(item[21]))) return FALSE;       // Y(t)
    if (!instream->get32bitsLE(&(item[25]))) return FALSE;       // Z(t)
    return TRUE;
  };
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
