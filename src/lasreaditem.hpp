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

  FILE:  LASreaditem.hpp
  
  CONTENTS:
  
    Common interface for all classes that read the items that compose a point.

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
#ifndef LAS_READ_ITEM_H
#define LAS_READ_ITEM_H

#include "mydefs.hpp"

class ByteStreamIn;

class LASreadItem
{
public:
  virtual BOOL read(U8* item)=0;

  virtual ~LASreadItem(){};
};

class LASreadItemRaw : public LASreadItem
{
public:
  LASreadItemRaw()
  {
    instream = 0;
  };
  BOOL init(ByteStreamIn* instream)
  {
    if (!instream) return FALSE;
    this->instream = instream;
    return TRUE;
  };
  virtual ~LASreadItemRaw(){};
protected:
  ByteStreamIn* instream;
};

class LASreadItemCompressed : public LASreadItem
{
public:
  virtual BOOL init(U8* item)=0;

  virtual ~LASreadItemCompressed(){};
};

#endif
