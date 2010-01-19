/*
===============================================================================

  FILE:  laspointwriter2raw.h
  
  CONTENTS:
  
    Writes a point of type 2 (without gps_time but with rgb color) in LAS format 1.2

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    7 September 2008 -- created after baking three apple pies with Kaoru 
  
===============================================================================
*/
#ifndef LAS_POINT_WRITER_2RAW_H
#define LAS_POINT_WRITER_2RAW_H

#include "laspointwriter.h"

#include <stdio.h>

class LASpointWriter2raw : public LASpointWriter
{
public:
  inline bool write_point(LASpoint* point, double gps_time = 0, unsigned short* rgb = 0)
  {
    fwrite(point, sizeof(LASpoint), 1, file);
    return (fwrite(rgb, sizeof(unsigned short), 3, file) == 3);
  };

  LASpointWriter2raw(FILE* file){this->file = file;};
  ~LASpointWriter2raw(){};
private:
  FILE* file;
};

#endif
