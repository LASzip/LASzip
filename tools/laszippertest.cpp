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

#if _MSC_VER < 1300
#include <istream.h>
#include <ostream.h>
#include <fstream.h>
#else
#include <fstream>
#endif


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  unsigned char c;
  unsigned int i, j;
  unsigned int num_points = 100000;
  unsigned int num_errors = 0;
  std::filebuf ofb1;
  std::filebuf ofb2;
  std::ostream* ostream1 = 0;
  std::ostream* ostream2 = 0;
  FILE* ofile1 = 0;
  FILE* ofile2 = 0;
  std::filebuf ifb1;
  std::filebuf ifb2;
  std::istream* istream1 = 0;
  std::istream* istream2 = 0;
  FILE* ifile1 = 0;
  FILE* ifile2 = 0;
  
  bool range = false;
  bool use_iostream = true;

  // describe the point structure

  unsigned int num_items = 4;
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

  // compute the point size
  unsigned int point_size = 0;
  for (i = 0; i < num_items; i++) point_size += items[i].size;

  // create the point data
  unsigned int point_offset = 0;
  unsigned char** point = new unsigned char*[num_items];
  unsigned char* point_data = new unsigned char[point_size];
  for (i = 0; i < num_items; i++)
  {
    point[i] = &(point_data[point_offset]);
    point_offset += items[i].size;
  }

  LASzipper* laszipper1 = new LASzipper(); // without compression
  LASzipper* laszipper2 = new LASzipper(); // with arithmetic compression

  if (use_iostream)
  {
    ofb1.open("test1.lax", std::ios::out);
    ostream1 = new std::ostream(&ofb1);
    if (!laszipper1->open(ostream1, num_items, items, LASZIP_COMPRESSION_NONE))
    {
      fprintf(stderr, "ERROR: could not open laszipper1\n");
      exit(1);
    }

    ofb2.open("test2.lax", std::ios::out);
    ostream2 = new std::ostream(&ofb2);
    if (!laszipper2->open(ostream2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC))
    {
      fprintf(stderr, "ERROR: could not open laszipper2\n");
      exit(1);
    }
  }
  else
  {
    ofile1 = fopen("test1.lax", "wb");
    if (!laszipper1->open(ofile1, num_items, items, LASZIP_COMPRESSION_NONE))
    {
      fprintf(stderr, "ERROR: could not open laszipper1\n");
      exit(1);
    }

    ofile2 = fopen("test2.lax", "wb");
    if (!laszipper2->open(ofile2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC))
    {
      fprintf(stderr, "ERROR: could not open laszipper2\n");
      exit(1);
    }
  }

  // compress 1000 points with "random" data

  c = 0;
  for (i = 0; i < num_points; i++)
  {
    for (j = 0; j < point_size; j++)
    {
      point_data[j] = c;
      c++;
    }
    laszipper1->write(point);
    laszipper2->write(point);
  }

  laszipper1->close();
  laszipper2->close();

  delete laszipper1;
  delete laszipper2;

  if (use_iostream)
  {
    ostream1.flush();
    ostream2.flush();
    delete ostream1;
    delete ostream2;
    ofb1.close();
    ofb2.close();
  }
  else
  {
    fclose(ofile1);
    fclose(ofile2);
  }

  LASunzipper* lasunzipper1 = new LASunzipper(); // without compression
  LASunzipper* lasunzipper2 = new LASunzipper(); // with arithmetic compression

  if (use_iostream)
  {
    ifb1.open("test1.lax", std::ios::in);
    istream1 = new std::istream(&ifb1);
    if (!lasunzipper1->open(istream1, num_items, items, LASZIP_COMPRESSION_NONE))
    {
      fprintf(stderr, "ERROR: could not open lasunzipper1\n");
      exit(1);
    }
    ifb2.open("test2.lax", std::ios::in);
    istream2 = new std::istream(&ifb2);
    if (!lasunzipper2->open(istream2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC))
    {
      fprintf(stderr, "ERROR: could not open lasunzipper2\n");
      exit(1);
    }
  }
  else
  {
    ifile1 = fopen("test1.lax", "rb");
    if (!lasunzipper1->open(ifile1, num_items, items, LASZIP_COMPRESSION_NONE))
    {
      fprintf(stderr, "ERROR: could not open lasunzipper1\n");
      exit(1);
    }
    ifile2 = fopen("test2.lax", "rb");
    if (!lasunzipper2->open(ifile2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC))
    {
      fprintf(stderr, "ERROR: could not open lasunzipper2\n");
      exit(1);
    }
  }

  // read 1000 points with "random" data

  num_errors = 0;
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    lasunzipper1->read(point);
    for (j = 0; j < point_size; j++)
    {
      if (point_data[j] != c) num_errors++;
      c++;
    }
  }
  if (num_errors) fprintf(stderr, "ERROR: with lasunzipper1 %d\n", num_errors);
  else fprintf(stderr, "SUCCESS: lasunzipper1 read %d points correclty\n", num_points);

  // decompress 1000 points with "random" data

  num_errors = 0;
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    lasunzipper2->read(point);
    for (j = 0; j < point_size; j++)
    {
      if (point_data[j] != c)
      {
        fprintf(stderr, "%d %d %d != %d\n", i, j, point_data[j], c);
        num_errors++;
        if (num_errors > 20) return -1;
      }
      c++;
    }
  }
  if (num_errors) fprintf(stderr, "ERROR: with lasunzipper2 %d\n", num_errors);
  else fprintf(stderr, "SUCCESS: lasunzipper2 read %d points correclty\n", num_points);

  lasunzipper1->close();
  lasunzipper2->close();

  delete lasunzipper1;
  delete lasunzipper2;

  if (use_iostream)
  {
    delete istream1;
    delete istream2;
    ifb1.close();
    ifb2.close();
  }
  else
  {
    fclose(ifile1);
    fclose(ifile2);
  }

  return 0;
}
