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

  FILE:  laszippertest.cpp
  
  CONTENTS:
  
    This tool reads and writes point data in the LAS 1.X format compressed
    or uncompressed to test the laszipper and lasunzipper interfaces.

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2010 martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    13 December 2010 -- created to test the remodularized laszip compressor
  
===============================================================================
*/

#include "laszipper.hpp"
#include "lasunzipper.hpp"

#include <istream.h>
#include <ostream.h>
#include <fstream.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int i, j;
  unsigned char c;
  bool range = false;
  int num_points = 1000;
  int num_errors = 0;

  // open the out files

  filebuf fb_out1;
  fb_out1.open("test1.lax", ios::out);
  ostream* ostream1 = new ostream(&fb_out1);

  filebuf fb_out2;
  fb_out2.open("test2.lax", ios::out);
  ostream* ostream2 = new ostream(&fb_out2);
  
  // describe the point structure

  int num_items = 1;
  LASitem items[4];

  items[0].type = LASitem::POINT10;
  items[0].size = 20;
  items[0].version = 0;

  items[1].type = LASitem::GPSTIME;
  items[1].size = 8;
  items[1].version = 0;

  items[2].type = LASitem::RGB;
  items[2].size = 6;
  items[2].version = 0;

  items[3].type = LASitem::BYTE;
  items[3].size = 7;
  items[3].version = 0;

  // create the point data

  unsigned char* point[4];
  unsigned char point_data[20+8+6+7];
  point[0] = &(point_data[0]);
  point[1] = &(point_data[20]);
  point[2] = &(point_data[28]);
  point[3] = &(point_data[34]);

  // create a zipper without compression

  LASzipper* laszipper1 = new LASzipper();
  if (!laszipper1->open(ostream1, num_items, items, LASZIP_COMPRESSION_NONE))
  {
    fprintf(stderr, "ERROR: could not open laszipper1\n");
    exit(1);
  }

  // create a zipper with arithmetic compression

  LASzipper* laszipper2 = new LASzipper();
  if (!laszipper2->open(ostream2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC))
  {
    fprintf(stderr, "ERROR: could not open laszipper2\n");
    exit(1);
  }

  // compress 1000 points with "random" data

  c = 0;
  for (i = 0; i < num_points; i++)
  {
    for (j = 0; j < (20+8+6+7); j++) point_data[j] = c++;
    laszipper1->write(point);
    laszipper2->write(point);
  }

  laszipper1->close();
  laszipper2->close();

  delete ostream1;
  delete ostream2;

  delete laszipper1;
  delete laszipper2;

  // open the in files

  filebuf fb_in1;
  fb_in1.open("test1.lax", ios::in);
  istream* istream1 = new istream(&fb_in1);

  // create an unzipper without compression

  LASunzipper* lasunzipper1 = new LASunzipper();
  if (!lasunzipper1->open(istream1, num_items, items, LASZIP_COMPRESSION_NONE))
  {
    fprintf(stderr, "ERROR: could not open lasunzipper1\n");
    exit(1);
  }

  // decompress 1000 points with "random" data

  num_errors = 0;
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    lasunzipper1->read(point);
    for (j = 0; j < (20+8+6+7); j++) if (point_data[j] != c++) num_errors++;
  }
  if (num_errors) fprintf(stderr, "ERROR: with lasunzipper1 %d\n", num_errors);

  // create an unzipper with arithmetic compression

  filebuf fb_in2;
  fb_in2.open("test2.lax", ios::in);
  istream* istream2 = new istream(&fb_in2);

  LASunzipper* lasunzipper2 = new LASunzipper();
  if (!lasunzipper2->open(istream2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC))
  {
    fprintf(stderr, "ERROR: could not open lasunzipper2\n");
    exit(1);
  }

  // decompress 1000 points with "random" data

  num_errors = 0;
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    lasunzipper2->read(point);
    for (j = 0; j < (20+8+6+7); j++) if (point_data[j] != c++) num_errors++;
  }
  if (num_errors) fprintf(stderr, "ERROR: with lasunzipper2 %d\n", num_errors);

  return 0;
}
