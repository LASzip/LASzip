/*
===============================================================================

  FILE:  LASreadPoint.hpp
  
  CONTENTS:
  
    Common interface for the classes that read points raw or compressed.

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
    7 December 2010 -- adapted from LASpointReader for better code modularity
    3 December 2010 -- updated to (somewhat) support LAS format 1.3
    7 September 2008 -- updated to support LAS format 1.2 
    22 February 2007 -- created about an hour before henna's birthday
  
===============================================================================
*/
#ifndef LAS_READ_POINT_H
#define LAS_READ_POINT_H

#include "mydefs.hpp"
#include "laszip.hpp"
#include "bytestreamin.hpp"

class LASreadItem;
class EntropyDecoder;

class LASreadPoint
{
public:
  LASreadPoint();
  ~LASreadPoint();

  // should only be called *once*
  BOOL setup(U32 num_items, const LASitem* items, LASzip::Algorithm algorithm);

  BOOL init(ByteStreamIn* instream);
  BOOL read(U8* const * point);
  BOOL done();

private:
  ByteStreamIn* instream;
  U32 num_readers;
  LASreadItem** readers;
  LASreadItem** readers_raw;
  LASreadItem** readers_compressed;
  EntropyDecoder* dec;
};

#endif
