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

  FILE:  lasunzipper.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/
#include "lasunzipper.hpp"

#include "bytestreamin_file.hpp"
#include "bytestreamin_istream.hpp"
#include "lasreadpoint.hpp"

unsigned int LASunzipper::open(FILE* infile, unsigned int num_items, const LASitem items[], LASzip::CompressionType compression_type)
{
  count = 0;
  stream = new ByteStreamInFile(infile);
  if (!stream) return 1;
  reader = new LASreadPoint();
  if (!reader) return 1;
  if (!reader->setup(num_items, items, compression_type)) return 1;
  if (!reader->init(stream)) return 1;
  return 0;
}

unsigned int LASunzipper::open(istream& instream, unsigned int num_items, const LASitem items[], LASzip::CompressionType compression_type)
{
  count = 0;
  stream = new ByteStreamInIstream(instream);
  if (!stream) return 1;
  reader = new LASreadPoint();
  if (!reader) return 1;
  if (!reader->setup(num_items, items, compression_type)) return 1;
  if (!reader->init(stream)) return 1;
  return 0;
}

bool LASunzipper::read(unsigned char * const * point)
{
  count++;
  return reader->read(point);
}

unsigned int LASunzipper::close()
{
  if (!reader->done()) return 1;
  count = 0;
  if (reader) delete reader;
  reader = 0;
  unsigned int byteCount = stream->byteCount();
  stream = 0;
  return byteCount;
}

LASunzipper::LASunzipper()
{
  count = 0;
  stream = 0;
  reader = 0;
}

LASunzipper::~LASunzipper()
{
  if (reader || stream) close();
}
