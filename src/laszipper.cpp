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

  FILE:  laszipper.cpp
  
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
#include "laszipper.hpp"

#include "bytestreamout_file.hpp"
#include "bytestreamout_ostream.hpp"
#include "laswritepoint.hpp"

bool LASzipper::open(FILE* outfile, unsigned int num_items, LASitem* items, unsigned int compression)
{
  count = 0;
  stream = new ByteStreamOutFile(outfile);
  if (!stream) return false;
  writer = new LASwritePoint();
  if (!writer) return false;
  if (!writer->setup(num_items, items, compression)) return false;
  if (!writer->init(stream)) return false;
  return true;
}

bool LASzipper::open(ostream* outstream, unsigned int num_items, LASitem* items, unsigned int compression)
{
  count = 0;
  stream = new ByteStreamOutOstream(outstream);
  if (!stream) return false;
  writer = new LASwritePoint();
  if (!writer) return false;
  if (!writer->setup(num_items, items, compression)) return false;
  if (!writer->init(stream)) return false;
  return true;
}

bool LASzipper::write(const unsigned char * const * point)
{
  count++;
  return writer->write(point);
}

/*
bool LASzipper::chunk(LASchunk* chunk)
{
  if (!writer->done()) return false;
  chunk->count = count;
  chunk->bytes = stream->byteCount();
  count = 0;
  stream->resetCount();
  if (!writer->init(stream)) return false;
  return true;
}
*/

/*
unsigned int LASzipper::close(LASchunk* chunk)
*/
unsigned int LASzipper::close()
{
  if (!writer->done()) return false;
/*
  if (chunk)
  {
    chunk->count = count;
    chunk->bytes = stream->byteCount();
  }
*/
  count = 0;
  if (writer) delete writer;
  writer = 0;
  unsigned int byteCount = stream->byteCount();
  if (stream) delete stream;
  stream = 0;
  return byteCount;
}

LASzipper::LASzipper()
{
  count = 0;
  stream = 0;
  writer = 0;
}

LASzipper::~LASzipper()
{
  if (writer || stream) close();
}
