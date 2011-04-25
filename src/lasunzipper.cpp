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

unsigned int LASunzipper::setup(const LASzip* laszip)
{
  if (!laszip) return 1;
  count = 0;
  if (reader) delete reader;
  reader = new LASreadPoint();
  if (!reader) return 1;
  if (!reader->setup(laszip->num_items, laszip->items, laszip)) return 1;
  return 0;
}

unsigned int LASunzipper::open(FILE* infile)
{
  if (!infile) return 1;
  if (!reader) return 1;
  if (stream) delete stream;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamInFileLE(infile);
  else
    stream = new ByteStreamInFileBE(infile);
  if (!stream) return 1;
  if (!reader->init(stream)) return 1;
  return 0;
}

unsigned int LASunzipper::open(istream& instream)
{
  if (!reader) return 1;
  if (stream) delete stream;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamInIstreamLE(instream);
  else
    stream = new ByteStreamInIstreamBE(instream);
  if (!stream) return 1;
  if (!reader->init(stream)) return 1;
  return 0;
}

bool LASunzipper::seek(const unsigned int position)
{
  if (reader->seek(count, position))
  {
    count = position;
    return true;
  }
  return false;
}

bool LASunzipper::read(unsigned char * const * point)
{
  count++;
  return (reader->read(point) == TRUE);
}

unsigned int LASunzipper::close()
{
  unsigned int byteCount = 0;
  if (reader)
  {
    reader->done();
    delete reader;
    reader = 0;
  }
  if (stream)
  {
    byteCount = stream->byteCount();
    delete stream;
    stream = 0;
  }
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
