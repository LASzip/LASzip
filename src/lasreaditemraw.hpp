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
    if (!instream->get32bits(&(item[ 0]))) return FALSE;       // x
    if (!instream->get32bits(&(item[ 4]))) return FALSE;       // y
    if (!instream->get32bits(&(item[ 8]))) return FALSE;       // z
    if (!instream->get16bits(&(item[12]))) return FALSE;       // intensity
    if (!instream->getBytes(&(item[14]), 4)) return FALSE;     // bitfield
                                                               // classification
                                                               // scan_angle_rank
                                                               // user_data
    if (!instream->get16bits(&(item[18]))) return FALSE;       // point_source_ID
    return TRUE;
  };
};

class LASreadItemRaw_GPSTIME : public LASreadItemRaw
{
public:
  LASreadItemRaw_GPSTIME(){};
  inline BOOL read(U8* item)
  {
    return instream->get64bits(item);                         // GPSTIME
  };
};

class LASreadItemRaw_RGB : public LASreadItemRaw
{
public:
  LASreadItemRaw_RGB(){};
  inline BOOL read(U8* item)
  {
    if (!instream->get16bits(&(item[0]))) return FALSE;        // R
    if (!instream->get16bits(&(item[2]))) return FALSE;        // G
    if (!instream->get16bits(&(item[4]))) return FALSE;        // B
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
