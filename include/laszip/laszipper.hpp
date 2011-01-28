/*
===============================================================================

  FILE:  LASzipper.hpp
  
  CONTENTS:
  
    Writes (optionally compressed) LIDAR points to LAS formats 1.0 - 1.3

  PROGRAMMERS:
  
    martin.isenburg@gmail.com
  
  COPYRIGHT:

    (c) 2010-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from LASwriter/LASreader after Howard got pushy (-;
  
===============================================================================
*/
#ifndef LAS_ZIPPER_H
#define LAS_ZIPPER_H

#include <stdio.h>

#include "laszip.hpp"

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
using namespace std;
#endif

class ByteStreamOut;
class LASwritePoint;

class LASZIP_DLL LASzipper
{
public:
  unsigned int open(FILE* outfile, unsigned int num_items, LASitem items[], LASzip::Algorithm algorithm);
  unsigned int open(ostream& outstream, unsigned int num_items, LASitem items[], LASzip::Algorithm algorithm);
  bool write(const unsigned char* const * point);
  unsigned int close();

  LASzipper();
  ~LASzipper();

private:
  int count;
  ByteStreamOut* stream;
  LASwritePoint* writer;
};

#endif
