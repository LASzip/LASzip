/******************************************************************************
 *
 * Project:  laszip - http://liblas.org - 
 * Purpose:  
 * Author:   Martin Isenburg
 *           martin.isenburg at gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2009, Martin Isenburg
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

  FILE:  LASpointReader.h
  
  CONTENTS:
  
    Common interface for the classes that read points raw or compressed.

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- updated to support LAS format 1.2 
    22 February 2007 -- created about an hour before henna's birthday
  
===============================================================================
*/
#ifndef LAS_POINT_READER_H
#define LAS_POINT_READER_H

#include "lasdefinitions.h"

class LASpointReader
{
public:
  virtual bool read_point(LASpoint* point, double* gps_time = 0, unsigned short* rgb = 0)=0;
  virtual ~LASpointReader(){};
};

#endif
