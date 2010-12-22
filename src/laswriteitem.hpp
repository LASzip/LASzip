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

  FILE:  LASwriteitem.hpp
  
  CONTENTS:
  
    Common interface for all classes that write the items that compose a point.

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
#ifndef LAS_WRITE_ITEM_H
#define LAS_WRITE_ITEM_H

#include "mydefs.hpp"

class ByteStreamOut;

class LASwriteItem
{
public:
  virtual BOOL write(const U8* item)=0;

  virtual ~LASwriteItem(){};
};

class LASwriteItemRaw : public LASwriteItem
{
public:
  LASwriteItemRaw()
  {
    outstream = 0;
  };
  BOOL init(ByteStreamOut* outstream)
  {
    if (!outstream) return FALSE;
    this->outstream = outstream;
    return TRUE;
  };
  virtual ~LASwriteItemRaw(){};
protected:
  ByteStreamOut* outstream;
};

class LASwriteItemCompressed : public LASwriteItem
{
public:
  virtual BOOL init(const U8* item)=0;

  virtual ~LASwriteItemCompressed(){};
};

#endif
