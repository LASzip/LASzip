/******************************************************************************
 *
 * Project:  laszip - http://liblas.org -
 * Purpose:
 * Author:   Martin Isenburg
 *           isenburg at cs.unc.edu
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

  FILE:  LASwriter.h
  
  CONTENTS:
  
    Writes LIDAR points to the LAS format (Version 1.x , April 29, 2008).

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- updated to support LAS format 1.2 
    21 February 2007 -- created after eating Sarah's veggies with peanutsauce
  
===============================================================================
*/
#ifndef LAS_WRITER_H
#define LAS_WRITER_H

#include "lasdefinitions.h"
#include "laspointwriter.h"

#include <stdio.h>

class LASwriter
{
public:
  int npoints;
  int p_count;

  bool open(FILE* file, LASheader* header, int compression = 0);

  bool write_point(LASpoint* point, double gps_time = 0, unsigned short* rgb = 0);
  bool write_point(double* coordinates);
  bool write_point(double x, double y, double z);

  int close(bool update_header=true);

  LASwriter();
  ~LASwriter();

private:
  FILE* file;
  LASpointWriter* pointWriter;
  LASheader* header;
  bool created_header;
};

#endif
