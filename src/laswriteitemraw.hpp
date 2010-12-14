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
  BOOL write(U8* item)
  {
    return outstream->putBytes(item, 20);
  };
};

class LASwriteItemRaw_GPSTIME : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_GPSTIME() {};
  BOOL write(U8* item)
  {
    return outstream->putBytes(item, 8);
  };
};

class LASwriteItemRaw_RGB : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_RGB(){}
  BOOL write(U8* item)
  {
    return outstream->putBytes(item, 6);
  };
};

class LASwriteItemRaw_BYTE : public LASwriteItemRaw
{
public:
  LASwriteItemRaw_BYTE(U32 number)
  {
    this->number = number;
  }
  BOOL write(U8* item)
  {
    return outstream->putBytes(item, number);
  };
private:
  U32 number;
};

#endif
