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
  BOOL read(U8* item)
  {
    return instream->getBytes(item, 20);
  };
};

class LASreadItemRaw_GPSTIME : public LASreadItemRaw
{
public:
  LASreadItemRaw_GPSTIME(){};
  BOOL read(U8* item)
  {
    return instream->getBytes(item, 8);
  };
};

class LASreadItemRaw_RGB : public LASreadItemRaw
{
public:
  LASreadItemRaw_RGB(){};
  BOOL read(U8* item)
  {
    return instream->getBytes(item, 6);
  };
};

class LASreadItemRaw_BYTE : public LASreadItemRaw
{
public:
  LASreadItemRaw_BYTE(U32 number)
  {
    this->number = number;
  }
  BOOL read(U8* item)
  {
    return instream->getBytes(item, number);
  };
private:
  U32 number;
};

#endif
