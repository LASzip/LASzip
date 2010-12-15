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

  FILE:  LASitemreadrawendianswapped.hpp
  
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
  
    14 December 2010 -- at Starbucks in Offenbach before watching "Fair Game"
  
===============================================================================
*/
#ifndef LAS_READ_ITEM_RAW_ENDIAN_SWAPPED_H
#define LAS_READ_ITEM_RAW_ENDIAN_SWAPPED_H

#include "lasreaditem.hpp"

#include <assert.h>

class LASreadItemRawEndianSwapped_POINT10 : public LASreadItemRaw
{
public:
  LASreadItemRawEndianSwapped_POINT10(){};
  BOOL read(U8* item)
  {
    U8 copy[20];
    BOOL flag = instream->getBytes(copy, 20);
    ENDIAN_SWAP_32(&(copy[ 0]), &(item[ 0]));   // x
    ENDIAN_SWAP_32(&(copy[ 4]), &(item[ 4]));   // y
    ENDIAN_SWAP_32(&(copy[ 8]), &(item[ 8]));   // z
    ENDIAN_SWAP_16(&(copy[12]), &(item[12]));   // intensity
    item[14] = copy[14];                        // bitfield
    item[15] = copy[15];                        // classification
    item[16] = copy[16];                        // scan_angle_rank
    item[17] = copy[17];                        // user_data
    ENDIAN_SWAP_16(&(copy[18]), &(item[18]));   // point_source_ID
    return flag;
  };
};

class LASreadItemRawEndianSwapped_GPSTIME : public LASreadItemRaw
{
public:
  LASreadItemRawEndianSwapped_GPSTIME(){};
  BOOL read(U8* item)
  {
    U8 copy[8];
    BOOL flag = instream->getBytes(copy, 8);
    ENDIAN_SWAP_64(copy, item);
    return flag;
  };
};

class LASreadItemRawEndianSwapped_RGB : public LASreadItemRaw
{
public:
  LASreadItemRawEndianSwapped_RGB(){};
  BOOL read(U8* item)
  {
    U8 copy[6];
    BOOL flag = instream->getBytes(copy, 6);
    ENDIAN_SWAP_16(&(copy[0]), &(item[0]));
    ENDIAN_SWAP_16(&(copy[2]), &(item[2]));
    ENDIAN_SWAP_16(&(copy[4]), &(item[4]));
    return flag;
  };
};

#endif
