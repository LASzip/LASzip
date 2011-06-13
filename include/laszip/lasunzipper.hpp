/*
===============================================================================

  FILE:  LASunzipper.hpp
  
  CONTENTS:
  
    Reads (optionally compressed) LIDAR points to LAS formats 1.0 - 1.3

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
  
    23 April 2011 -- changed interface for easier future compressor support
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from LASwriter/LASreader after Howard got pushy (-;
  
===============================================================================
*/
#ifndef LAS_UNZIPPER_HPP
#define LAS_UNZIPPER_HPP

#include <stdio.h>

#include "laszip.hpp"

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
using namespace std;
#endif

class ByteStreamIn;
class LASreadPoint;

class LASZIP_DLL LASunzipper
{
public:
  bool open(FILE* file, const LASzip* laszip);
  bool open(istream& stream, const LASzip* laszip);
 
  unsigned int tell() const;
  bool seek(const unsigned int position);
  bool read(unsigned char * const * point);
  bool close();

  LASunzipper();
  ~LASunzipper();

  char* error_string;

private:
  unsigned int count;
  ByteStreamIn* stream;
  LASreadPoint* reader;
};

#endif
