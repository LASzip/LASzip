/*
===============================================================================

  FILE:  LASreader.h
  
  CONTENTS:
  
    Reads LIDAR points from the LAS format (Version 1.x , April 29, 2008).

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- updated to support LAS format 1.2 
    18 February 2007 -- created after repairing 2 vacuum cleaners in the garden
  
===============================================================================
*/
#ifndef LAS_READER_H
#define LAS_READER_H

#include "lasdefinitions.h"
#include "laspointreader.h"

#include <stdio.h>

class LASreader
{
public:
  LASheader header;

  LASpoint point;
  double gps_time;
  unsigned short rgb[3];

  bool points_have_gps_time;
  bool points_have_rgb;

  int npoints;
  int p_count;

  int additional_bytes_per_point;

  bool open(FILE* file, bool skip_all_headers = false);

  bool read_point();
  bool read_point(float* coordinates);
  bool read_point(double* coordinates);

  void get_coordinates(float* coordinates);
  void get_coordinates(double* coordinates);

  void close();

  LASreader();
  ~LASreader();

private:
  FILE* file;
  LASpointReader* pointReader;
};

#endif
