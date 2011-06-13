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

#include <string.h>
#include <stdlib.h>

#include "bytestreamin_file.hpp"
#include "bytestreamin_istream.hpp"
#include "lasreadpoint.hpp"

bool LASunzipper::open(FILE* infile, const LASzip* laszip)
{
  if (!infile) { error_string = strdup("FILE* infile pointer is NULL"); return false; };
  if (!laszip) { error_string = strdup("const LASzip* laszip pointer is NULL"); return false; };
  count = 0;
  if (reader) delete reader;
  reader = new LASreadPoint();
  if (!reader) { error_string = strdup("alloc of LASreadPoint failed"); return false; };
  if (!reader->setup(laszip->num_items, laszip->items, laszip)) { error_string = strdup("setup() of LASreadPoint failed"); return false; };
  if (stream) delete stream;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamInFileLE(infile);
  else
    stream = new ByteStreamInFileBE(infile);
  if (!stream) { error_string = strdup("alloc of ByteStreamInFile failed"); return false; };
  if (!reader->init(stream)) { error_string = strdup("init() of LASreadPoint failed"); return false; };
  return true;
}

bool LASunzipper::open(istream& instream, const LASzip* laszip)
{
  if (!laszip) { error_string = strdup("const LASzip* laszip pointer is NULL"); return false; };
  count = 0;
  if (reader) delete reader;
  reader = new LASreadPoint();
  if (!reader) { error_string = strdup("alloc of LASreadPoint failed"); return false; };
  if (!reader->setup(laszip->num_items, laszip->items, laszip)) { error_string = strdup("setup() of LASreadPoint failed"); return false; };
  if (stream) delete stream;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamInIstreamLE(instream);
  else
    stream = new ByteStreamInIstreamBE(instream);
  if (!stream) { error_string = strdup("alloc of ByteStreamInStream failed"); return false; };
  if (!reader->init(stream)) { error_string = strdup("init() of LASreadPoint failed"); return false; };
  return true;
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

unsigned int LASunzipper::tell() const
{
  return count;
}

bool LASunzipper::read(unsigned char * const * point)
{
  count++;
  return (reader->read(point) == TRUE);
}

bool LASunzipper::close()
{
  BOOL done = TRUE;
  if (reader)
  {
    done = reader->done();
    delete reader;
    reader = 0;
  }
  if (stream)
  {
    delete stream;
    stream = 0;
  }
  if (!done) { error_string = strdup("done() of LASreadPoint failed"); return false; }
  return true;
}

LASunzipper::LASunzipper()
{
  error_string = 0;
  count = 0;
  stream = 0;
  reader = 0;
}

LASunzipper::~LASunzipper()
{
  if (error_string) free(error_string);
  if (reader || stream) close();
}
