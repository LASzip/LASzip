/*
===============================================================================

  FILE:  lasunzipper.cpp
  
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
#include "lasunzipper.hpp"

#include "bytestreamin_file.hpp"
#include "bytestreamin_istream.hpp"
#include "lasreadpoint.hpp"

unsigned int LASunzipper::open(FILE* infile, unsigned int num_items, const LASitem items[], LASzip::Algorithm algorithm)
{
  count = 0;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamInFileLE(infile);
  else
    stream = new ByteStreamInFileBE(infile);
  if (!stream) return 1;
  reader = new LASreadPoint();
  if (!reader) return 1;
  if (!reader->setup(num_items, items, algorithm)) return 1;
  if (!reader->init(stream)) return 1;
  return 0;
}

unsigned int LASunzipper::open(istream& instream, unsigned int num_items, const LASitem items[], LASzip::Algorithm algorithm)
{
  count = 0;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamInIstreamLE(instream);
  else
    stream = new ByteStreamInIstreamBE(instream);
  if (!stream) return 1;
  reader = new LASreadPoint();
  if (!reader) return 1;
  if (!reader->setup(num_items, items, algorithm)) return 1;
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
  delete stream;
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
