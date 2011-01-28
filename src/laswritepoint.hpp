/*
===============================================================================

  FILE:  LASwritePoint.hpp
  
  CONTENTS:
  
    Common interface for the classes that write points raw or compressed.

  PROGRAMMERS:
  
    martin.isenburg@gmail.com
  
  COPYRIGHT:

    (c) 2007-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
    7 December 2010 -- adapted from LASpointWriter for better code modularity
    3 December 2010 -- updated to (somewhat) support LAS format 1.3
    7 September 2008 -- updated to support LAS format 1.2 
    22 February 2007 -- created about an hour before henna's birthday
  
===============================================================================
*/
#ifndef LAS_WRITE_POINT_H
#define LAS_WRITE_POINT_H

#include "mydefs.hpp"
#include "laszip.hpp"
#include "bytestreamout.hpp"

class LASwriteItem;
class EntropyEncoder;

class LASwritePoint
{
public:
  LASwritePoint();
  ~LASwritePoint();

  // should only be called *once*
  BOOL setup(U32 num_items, LASitem* items, LASzip::Algorithm algorithm);

  BOOL init(ByteStreamOut* outstream);
  BOOL write(const U8 * const * point);
  BOOL done();

private:
  ByteStreamOut* outstream;
  U32 num_writers;
  LASwriteItem** writers;
  LASwriteItem** writers_raw;
  LASwriteItem** writers_compressed;
  EntropyEncoder* enc;
};

#endif
