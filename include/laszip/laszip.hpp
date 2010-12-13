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

  FILE:  laszip.hpp
  
  CONTENTS:
  
    Contains LASitem and LASchunk structs as well as the IDs of the currently
    supported entropy coding scheme

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    12 December 2010 -- refactored from lasdefinitions after movies with silke
  
===============================================================================
*/
#ifndef LASZIP_H
#define LASZIP_H

#define LASZIP_COMPRESSION_NONE 0
#define LASZIP_COMPRESSION_RANGE 1
#define LASZIP_COMPRESSION_ARITHMETIC 2

struct LASitem
{
  enum { BYTE, SHORT, INT, LONG, FLOAT, DOUBLE, POINT10, GPSTIME, RGB } type;
  short size;
  short version;
};

struct LASchunk
{
  unsigned int count;
  unsigned int bytes;
};

#endif
