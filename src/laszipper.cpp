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

#include <string.h>
#include <stdlib.h>

#include "bytestreamout_file.hpp"
#include "bytestreamout_ostream.hpp"
#include "laswritepoint.hpp"

bool LASzipper::open(FILE* outfile, const LASzip* laszip)
{
  if (!outfile) { error_string = strdup("FILE* outfile pointer is NULL"); return false; };
  if (!laszip) { error_string = strdup("const LASzip* laszip pointer is NULL"); return false; };
  count = 0;
  if (writer) delete writer;
  writer = new LASwritePoint();
  if (!writer) { error_string = strdup("alloc of LASwritePoint failed"); return false; };
  if (!writer->setup(laszip->num_items, laszip->items, laszip)) { error_string = strdup("setup() of LASwritePoint failed"); return false; };
  if (stream) delete stream;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamOutFileLE(outfile);
  else
    stream = new ByteStreamOutFileBE(outfile);
  if (!stream) { error_string = strdup("alloc of ByteStreamOutFile failed"); return false; };
  if (!writer->init(stream)) { error_string = strdup("init() of LASwritePoint failed"); return false; };
  return true;
}

bool LASzipper::open(ostream& outstream, const LASzip* laszip)
{
  if (!laszip) { error_string = strdup("const LASzip* laszip pointer is NULL"); return false; };
  count = 0;
  if (writer) delete writer;
  writer = new LASwritePoint();
  if (!writer) { error_string = strdup("alloc of LASwritePoint failed"); return false; };
  if (!writer->setup(laszip->num_items, laszip->items, laszip)) { error_string = strdup("setup() of LASwritePoint failed"); return false; };
  if (stream) delete stream;
  if (IS_LITTLE_ENDIAN())
    stream = new ByteStreamOutOstreamLE(outstream);
  else
    stream = new ByteStreamOutOstreamBE(outstream);
  if (!stream) { error_string = strdup("alloc of ByteStreamOutStream failed"); return false; };
  if (!writer->init(stream)) { error_string = strdup("init() of LASwritePoint failed"); return false; };
  return true;
}

bool LASzipper::write(const unsigned char * const * point)
{
  count++;
  return (writer->write(point) == TRUE);
}

bool LASzipper::chunk()
{
  return (writer->chunk() == TRUE);
}

bool LASzipper::close()
{
  BOOL done = TRUE;
  if (writer)
  {
    BOOL done = writer->done();
    delete writer;
    writer = 0;
  }
  if (stream)
  {
    delete stream;
    stream = 0;
  }
  if (!done) { error_string = strdup("done() of LASwritePoint failed"); return false; }
  return true;
}

LASzipper::LASzipper()
{
  error_string = 0;
  count = 0;
  stream = 0;
  writer = 0;
}

LASzipper::~LASzipper()
{
  if (error_string) free(error_string);
  if (writer || stream) close();
}
