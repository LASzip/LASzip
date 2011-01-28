/*
===============================================================================

  FILE:  laszipper.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:

    martin.isenburg@gmail.com

  COPYRIGHT:

    (c) 2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/
#include "laszipper.hpp"

#include "bytestreamout_file.hpp"
#include "bytestreamout_ostream.hpp"
#include "laswritepoint.hpp"

unsigned int LASzipper::open(FILE* outfile, unsigned int num_items, LASitem items[], LASzip::Algorithm algorithm)
{
  count = 0;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamOutFileLE(outfile);
  else
    stream = new ByteStreamOutFileBE(outfile);
  if (!stream) return 1;
  writer = new LASwritePoint();
  if (!writer) return 1;
  if (!writer->setup(num_items, items, algorithm)) return 1;
  if (!writer->init(stream)) return 1;
  return 0;
}

unsigned int LASzipper::open(ostream& outstream, unsigned int num_items, LASitem items[], LASzip::Algorithm algorithm)
{
  count = 0;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamOutOstreamLE(outstream);
  else
    stream = new ByteStreamOutOstreamBE(outstream);
  if (!stream) return 1;
  writer = new LASwritePoint();
  if (!writer) return 1;
  if (!writer->setup(num_items, items, algorithm)) return 1;
  if (!writer->init(stream)) return 1;
  return 0;
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
  delete stream;
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
