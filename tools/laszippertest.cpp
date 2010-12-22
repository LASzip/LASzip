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

#ifdef LZ_WIN32_VC6
    #include <fstream.h>
#else
    #include <istream>
    #include <fstream>
    using namespace std;
#endif

#include <time.h>
#include <stdio.h>

static double taketime()
{
  return (double)(clock())/CLOCKS_PER_SEC;
}

int main(int argc, char *argv[])
{
  unsigned char c;
  unsigned int i, j;
  unsigned int num_points = 1000000;
  unsigned int num_errors, num_bytes;
  filebuf ofb1;
  filebuf ofb2;
  filebuf ofb3;


#ifdef LZ_WIN32_VC6
  ostream* ostream1 = 0;
  ostream* ostream2 = 0;
  ostream* ostream3 = 0;
  istream* istream1 = 0;
  istream* istream2 = 0;
  istream* istream3 = 0;  
#else
  ofstream* ostream1 = 0;
  ofstream* ostream2 = 0;
  ofstream* ostream3 = 0;
  ifstream* istream1 = 0;
  ifstream* istream2 = 0;
  ifstream* istream3 = 0;  
#endif



  FILE* ofile1 = 0;
  FILE* ofile2 = 0;
  FILE* ofile3 = 0;
  filebuf ifb1;
  filebuf ifb2;
  filebuf ifb3;

  FILE* ifile1 = 0;
  FILE* ifile2 = 0;
  FILE* ifile3 = 0;
  double start_time, end_time;
  
  bool use_iostream = true;

  // describe the point structure

  unsigned int num_items = 4;
  LASitem items[4];

  items[0].type = LASitem::POINT10;
  items[0].size = 20;
  items[0].version = 0;

  items[1].type = LASitem::GPSTIME11;
  items[1].size = 8;
  items[1].version = 0;

  items[2].type = LASitem::RGB12;
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
  LASzipper* laszipper3 = new LASzipper(); // with range compression

  if (use_iostream)
  {

#ifdef LZ_WIN32_VC6
    ofb1.open("test1.lax", ios::out);
    ofb1.setmode(filebuf::binary);
    ostream1 = new ostream(&ofb1);
#else
    ostream1 = new ofstream();
    ostream1->open("test1.lax", std::ios::out | std::ios::binary );
#endif



    if (laszipper1->open(*ostream1, num_items, items, LASZIP_COMPRESSION_NONE) != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper1\n");
      return 0;
    }

#ifdef LZ_WIN32_VC6
    ofb2.open("test2.lax", ios::out);
    ofb2.setmode(filebuf::binary);
    ostream2 = new ostream(&ofb2);
#else
    ostream2 = new ofstream();
    ostream2->open("test2.lax", std::ios::out | std::ios::binary );
#endif

    if (laszipper2->open(*ostream2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC) != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper2\n");
      return 0;
    }

#ifdef LZ_WIN32_VC6
    ofb3.open("test3.lax", ios::out);
    ofb3.setmode(filebuf::binary);
    ostream3 = new ostream(&ofb3);
#else
    ostream3 = new ofstream();
    ostream3->open("test3.lax", std::ios::out | std::ios::binary );
#endif


    if (laszipper3->open(*ostream3, num_items, items, LASZIP_COMPRESSION_RANGE) != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper3\n");
      return 0;
    }
  }
  else
  {
    ofile1 = fopen("test1.lax", "wb");
    if (laszipper1->open(ofile1, num_items, items, LASZIP_COMPRESSION_NONE) != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper1\n");
      return 0;
    }

    ofile2 = fopen("test2.lax", "wb");
    if (laszipper2->open(ofile2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC) != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper2\n");
      return 0;
    }

    ofile3 = fopen("test3.lax", "wb");
    if (laszipper3->open(ofile3, num_items, items, LASZIP_COMPRESSION_RANGE) != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper3\n");
      return 0;
    }
  }

  // write / compress num_points with "random" data

  start_time = taketime();
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    for (j = 0; j < point_size; j++)
    {
      point_data[j] = c;
      c++;
    }
    laszipper1->write(point);
  }
  num_bytes = laszipper1->close();
  end_time = taketime();
  fprintf(stderr, "laszipper1 wrote %d bytes in %g seconds\n", num_bytes, end_time-start_time);

  start_time = taketime();
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    for (j = 0; j < point_size; j++)
    {
      point_data[j] = c;
      c++;
    }
    laszipper2->write(point);
  }
  num_bytes = laszipper2->close();
  end_time = taketime();
  fprintf(stderr, "laszipper2 wrote %d bytes in %g seconds\n", num_bytes, end_time-start_time);

  start_time = taketime();
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    for (j = 0; j < point_size; j++)
    {
      point_data[j] = c;
      c++;
    }
    laszipper3->write(point);
  }
  num_bytes = laszipper3->close();
  end_time = taketime();
  fprintf(stderr, "laszipper3 wrote %d bytes in %g seconds\n", num_bytes, end_time-start_time);

  delete laszipper1;
  delete laszipper2;
  delete laszipper3;

  if (use_iostream)
  {
    delete ostream1;
    delete ostream2;
    delete ostream3;
    ofb1.close();
    ofb2.close();
    ofb3.close();
  }
  else
  {
    fclose(ofile1);
    fclose(ofile2);
    fclose(ofile3);
  }

  LASunzipper* lasunzipper1 = new LASunzipper(); // without compression
  LASunzipper* lasunzipper2 = new LASunzipper(); // with arithmetic compression
  LASunzipper* lasunzipper3 = new LASunzipper(); // with range compression

  if (use_iostream)
  {

#ifdef LZ_WIN32_VC6
    ifb1.open("test1.lax", ios::in);
    ifb1.setmode(filebuf::binary);
    istream1 = new istream(&ifb1);
#else
    istream1 = new ifstream();
    istream1->open("test1.lax", std::ios::in | std::ios::binary );
#endif


    if (lasunzipper1->open(*istream1, num_items, items, LASZIP_COMPRESSION_NONE) != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper1\n");
      return 0;
    }

#ifdef LZ_WIN32_VC6
    ifb2.open("test2.lax", ios::in);
    ifb2.setmode(filebuf::binary);
    istream2 = new istream(&ifb2);
#else
    istream2 = new ifstream();
    istream2->open("test2.lax", std::ios::in | std::ios::binary );
#endif

    if (lasunzipper2->open(*istream2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC) != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper2\n");
      return 0;
    }

#ifdef LZ_WIN32_VC6
    ifb3.open("test3.lax", ios::in);
    ifb3.setmode(filebuf::binary);
    istream3 = new istream(&ifb3);
#else
    istream3 = new ifstream();
    istream3->open("test3.lax", std::ios::in | std::ios::binary );
#endif

    if (lasunzipper3->open(*istream3, num_items, items, LASZIP_COMPRESSION_RANGE) != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper3\n");
      return 0;
    }
  }
  else
  {
    ifile1 = fopen("test1.lax", "rb");
    if (lasunzipper1->open(ifile1, num_items, items, LASZIP_COMPRESSION_NONE) != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper1\n");
      return 0;
    }
    ifile2 = fopen("test2.lax", "rb");
    if (lasunzipper2->open(ifile2, num_items, items, LASZIP_COMPRESSION_ARITHMETIC) != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper2\n");
      return 0;
    }
    ifile3 = fopen("test3.lax", "rb");
    if (lasunzipper3->open(ifile3, num_items, items, LASZIP_COMPRESSION_RANGE) != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper3\n");
      return 0;
    }
  }

  // read num_points with "random" data

  start_time = taketime();
  num_errors = 0;
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    lasunzipper1->read(point);
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
  num_bytes = lasunzipper1->close();
  end_time = taketime();
  if (num_errors)
  {
    fprintf(stderr, "ERROR: with lasunzipper1 %d\n", num_errors);
  }
  else
  {
   fprintf(stderr, "SUCCESS: lasunzipper1 read %d bytes in %g seconds\n", num_bytes, end_time-start_time);
  }

  // decompress num_points with "random" data

  start_time = taketime();
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
  num_bytes = lasunzipper2->close();
  end_time = taketime();
  if (num_errors)
  {
    fprintf(stderr, "ERROR: with lasunzipper2 %d\n", num_errors);
  }
  else
  {
   fprintf(stderr, "SUCCESS: lasunzipper2 read %d bytes in %g seconds\n", num_bytes, end_time-start_time);
  }

  // decompress num_points with "random" data

  start_time = taketime();
  num_errors = 0;
  c = 0;
  for (i = 0; i < num_points; i++)
  {
    lasunzipper3->read(point);
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
  num_bytes = lasunzipper3->close();
  end_time = taketime();
  if (num_errors)
  {
    fprintf(stderr, "ERROR: with lasunzipper3 %d\n", num_errors);
  }
  else
  {
   fprintf(stderr, "SUCCESS: lasunzipper3 read %d bytes in %g seconds\n", num_bytes, end_time-start_time);
  }

  delete lasunzipper1;
  delete lasunzipper2;
  delete lasunzipper3;

  if (use_iostream)
  {
    ifb1.close();
    ifb2.close();
    ifb3.close();
    delete istream1;
    delete istream2;
    delete istream3;
  }
  else
  {
    fclose(ifile1);
    fclose(ifile2);
    fclose(ifile3);
  }

  return 0;
}
