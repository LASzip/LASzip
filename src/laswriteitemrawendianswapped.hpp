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

  FILE:  LASwriteitemrawendianswapped.hpp
  
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
  
    14 December 2010 -- at Starbucks in Offenbach before watching "Fair Game"
  
===============================================================================
*/
#ifndef LAS_WRITE_ITEM_RAW_WITH_ENDIAN_SWAPPED_H
#define LAS_WRITE_ITEM_RAW_WITH_ENDIAN_SWAPPED_H

#include "laswriteitem.hpp"

#include <assert.h>

class LASwriteItemRawEndianSwapped_POINT10 : public LASwriteItemRaw
{
public:
  LASwriteItemRawEndianSwapped_POINT10(){};
  BOOL write(U8* item)
  {
    U8 copy[20];
    ENDIAN_SWAP_32(&(item[ 0]), &(copy[ 0]));   // x
    ENDIAN_SWAP_32(&(item[ 4]), &(copy[ 4]));   // y
    ENDIAN_SWAP_32(&(item[ 8]), &(copy[ 8]));   // z
    ENDIAN_SWAP_16(&(item[12]), &(copy[12]));   // intensity
    copy[14] = item[14];                        // bitfield
    copy[15] = item[15];                        // classification
    copy[16] = item[16];                        // scan_angle_rank
    copy[17] = item[17];                        // user_data
    ENDIAN_SWAP_16(&(item[18]), &(copy[18]));   // point_source_ID
    return outstream->putBytes(copy, 20);
  };
};

class LASwriteItemRawEndianSwapped_GPSTIME : public LASwriteItemRaw
{
public:
  LASwriteItemRawEndianSwapped_GPSTIME() {};
  BOOL write(U8* item)
  {
    U8 copy[8];
    ENDIAN_SWAP_64(item, copy);
    return outstream->putBytes(copy, 8);
  };
};

class LASwriteItemRawEndianSwapped_RGB : public LASwriteItemRaw
{
public:
  LASwriteItemRawEndianSwapped_RGB(){}
  BOOL write(U8* item)
  {
    U8 copy[6];
    ENDIAN_SWAP_16(&(item[0]), &(copy[0]));
    ENDIAN_SWAP_16(&(item[2]), &(copy[2]));
    ENDIAN_SWAP_16(&(item[4]), &(copy[4]));
    return outstream->putBytes(copy, 6);
  };
};

#endif
