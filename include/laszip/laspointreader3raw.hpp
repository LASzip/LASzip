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

  FILE:  laspointreader3raw.h
  
  CONTENTS:
  
    Reads a point of type 3 (with gps_time and rgb colors) in LAS format 1.2

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- created after baking three apple pies with Kaoru 
  
===============================================================================
*/
#ifndef LAS_POINT_READER_3RAW_H
#define LAS_POINT_READER_3RAW_H

#include "laspointreader.hpp"

#include <stdio.h>

class LASpointReader3raw : public LASpointReader
{
public:
  inline bool read_point(LASpoint* point, double* gps_time = 0, unsigned short* rgb = 0)
  {
    fread(point, sizeof(LASpoint), 1, file);
    fread(gps_time, sizeof(double), 1, file);
    return (fread(rgb, sizeof(unsigned short), 3, file) == 3);
  };
  LASpointReader3raw(FILE* file){this->file = file;};
  ~LASpointReader3raw(){};
private:
  FILE* file;
};

#endif
