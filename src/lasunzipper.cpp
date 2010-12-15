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


bool LASunzipper::open(FILE* infile, unsigned int num_items, const LASitem* items, unsigned int compression)
{
  count = 0;
  stream = new ByteStreamInFile(infile);
  if (!stream) return false;
  reader = new LASreadPoint();
  if (!reader) return false;
  if (!reader->setup(num_items, items, compression)) return false;
  if (!reader->init(stream)) return false;
  return true;
}

bool LASunzipper::open(istream* instream, unsigned int num_items, const LASitem* items, unsigned int compression)
{
  count = 0;
  stream = new ByteStreamInIstream(instream);
  if (!stream) return false;
  reader = new LASreadPoint();
  if (!reader) return false;
  if (!reader->setup(num_items, items, compression)) return false;
  if (!reader->init(stream)) return false;
  return true;
}

bool LASunzipper::read(unsigned char** point)
{
  count++;
  return reader->read(point);
}

bool LASunzipper::close()
{
  if (!reader->done()) return false;
  count = 0;
  if (reader) delete reader;
  reader = 0;
  if (stream) delete stream;
  stream = 0;
  return true;
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
