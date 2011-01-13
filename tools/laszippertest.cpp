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

//#define LASZIP_HAVE_RANGECODER

static double taketime()
{
  return (double)(clock())/CLOCKS_PER_SEC;
}

// abstractions for doing I/O, which support VC6 streams, modern streams, and FILE*
struct OStream
{
public:
    OStream(bool use_iostream, const char* filename) :
      m_use_iostream(use_iostream),
      m_filename(filename),
      ofile(NULL),
      ostream(NULL)
    {
        if (m_use_iostream)
        {
#ifdef LZ_WIN32_VC6 
            ofb.open(filename, ios::out);
            ofb.setmode(filebuf::binary);
            ostream = new ostream(&ofb);
#else
            ostream = new ofstream();
            ostream->open(filename, std::ios::out | std::ios::binary );
#endif 
        }
        else
        {
            ofile = fopen(filename, "wb");
        }

        return;
    }

      ~OStream()
      {
          if (m_use_iostream)
          {
              delete ostream;
#ifdef LZ_WIN32_VC6 
              ofb.close();
#endif
          }
          else
          {
              if (ofile)
                  fclose(ofile);
          }
      }

public:
    bool m_use_iostream;
    const char* m_filename;
    FILE* ofile;
    filebuf ofb;
#ifdef LZ_WIN32_VC6 
    ostream* ostream;
#else
    ofstream* ostream;
#endif
};


struct IStream
{
public:
    IStream(bool use_iostream, const char* filename) :
      m_use_iostream(use_iostream),
      m_filename(filename),
      ifile(NULL),
      istream(NULL)
    {
        if (m_use_iostream)
        {
#ifdef LZ_WIN32_VC6 
            ifb.open(filename, ios::in);
            ifb.setmode(filebuf::binary);
            istream = new istream(&ifb);
#else
            istream = new ifstream();
            istream->open(filename, std::ios::in | std::ios::binary );
#endif 
        }
        else
        {
            ifile = fopen(filename, "rb");
        }
      }

      ~IStream()
      {
          if (m_use_iostream)
          {
              delete istream;
#ifdef LZ_WIN32_VC6 
              ifb.close();
#endif
          }
          else
          {
              if (ifile)
                  fclose(ifile);
          }
      }

public:
    const char* m_filename;
    bool m_use_iostream;
    FILE* ifile;
    filebuf ifb;
#ifdef LZ_WIN32_VC6 
    istream* istream;
#else
    ifstream* istream;
#endif
};


static LASzipper* make_zipper(OStream* ost, unsigned int num_items, LASitem items[], LASzip::Algorithm alg)
{
    LASzipper* zipper = new LASzipper();

    int stat = 0;
    if (ost->m_use_iostream)
        stat = zipper->open(*ost->ostream, num_items, items, alg);
    else
        stat = zipper->open(ost->ofile, num_items, items, alg);

    if (stat != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper with %s\n", ost->m_filename);
      exit(1);
    }

    return zipper;
}

static LASunzipper* make_unzipper(IStream* ist, unsigned int num_items, LASitem items[], LASzip::Algorithm alg)
{
    LASunzipper* unzipper = new LASunzipper();

    int stat = 0;
    if (ist->m_use_iostream)
        stat = unzipper->open(*ist->istream, num_items, items, alg);
    else
        stat = unzipper->open(ist->ifile, num_items, items, alg);

    if (stat != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper with %s\n", ist->m_filename);
      exit(1);
    }

    return unzipper;
}


static void write_points(LASzipper* zipper, unsigned int num_points, 
                         unsigned int point_size, unsigned char* point_data, unsigned char** point)
{
    double start_time, end_time;
    unsigned char c;
    unsigned int i,j;

    start_time = taketime();
    c = 0;
    for (i = 0; i < num_points; i++)
    {
        for (j = 0; j < point_size; j++)
        {
            point_data[j] = c;
            c++;
        }
        zipper->write(point);
    }

    unsigned int num_bytes = zipper->close();
    end_time = taketime();
    fprintf(stderr, "laszipper wrote %d bytes in %g seconds\n", num_bytes, end_time-start_time);

    return;
}

static void read_points(LASunzipper* unzipper, unsigned int num_points,
                        unsigned int point_size, unsigned char* point_data, unsigned char** point)
{
    unsigned char c;
    unsigned int i,j;
    unsigned int num_errors, num_bytes;
    double start_time, end_time;

    start_time = taketime();
    num_errors = 0;

    c = 0;
    for (i = 0; i < num_points; i++)
    {
        unzipper->read(point);
        for (j = 0; j < point_size; j++)
        {
            if (point_data[j] != c)
            {
                fprintf(stderr, "%d %d %d != %d\n", i, j, point_data[j], c);
                num_errors++;
                if (num_errors > 20) break;
            }
            c++;
        }
        if (num_errors > 20) break;
    }
    num_bytes = unzipper->close();
    end_time = taketime();
    if (num_errors)
    {
        fprintf(stderr, "ERROR: with lasunzipper %d\n", num_errors);
    }
    else
    {
        fprintf(stderr, "SUCCESS: lasunzipper read %d bytes in %g seconds\n", num_bytes, end_time-start_time);
    }

    return;
}

int main(int argc, char *argv[])
{
  unsigned int i;

  unsigned int num_points = 100000;
  bool use_iostream = false;

  // describe the point structure

  unsigned int num_items = 5;
  LASitem items[5];

  items[0].type = LASitem::POINT10;
  items[0].size = 20;
  items[0].version = 0;

  items[1].type = LASitem::GPSTIME11;
  items[1].size = 8;
  items[1].version = 0;

  items[2].type = LASitem::RGB12;
  items[2].size = 6;
  items[2].version = 0;

  items[3].type = LASitem::WAVEPACKET13;
  items[3].size = 29;
  items[3].version = 0;

  items[4].type = LASitem::BYTE;
  items[4].size = 7;
  items[4].version = 0;

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

    OStream* ost1 = new OStream(use_iostream, "test1.lax");
    OStream* ost2 = new OStream(use_iostream, "test2.lax");
    OStream* ost3 = new OStream(use_iostream, "test3.lax");

    LASzipper* laszipper1 = make_zipper(ost1, num_items, items, LASzip::POINT_BY_POINT_RAW);
    LASzipper* laszipper2 = make_zipper(ost2, num_items, items, LASzip::POINT_BY_POINT_ARITHMETIC);
#ifdef LASZIP_HAVE_RANGECODER
    LASzipper* laszipper3 = make_zipper(ost3, num_items, items, LASzip::POINT_BY_POINT_RANGE);
#else
    fprintf(stderr, "(skipping range coder test)\n");
#endif

  // write / compress num_points with "random" data

  write_points(laszipper1, num_points, point_size, point_data, point);
  
  write_points(laszipper2, num_points, point_size, point_data, point); 
#ifdef LASZIP_HAVE_RANGECODER
  write_points(laszipper3, num_points, point_size, point_data, point);
#endif
  
  delete laszipper1;
  delete laszipper2;
#ifdef LASZIP_HAVE_RANGECODER
  delete laszipper3;
#endif

  delete ost1;
  delete ost2;
  delete ost3;

  IStream* ist1 = new IStream(use_iostream, "test1.lax");
  IStream* ist2 = new IStream(use_iostream, "test2.lax");
  IStream* ist3 = new IStream(use_iostream, "test3.lax");

  LASunzipper* lasunzipper1 = make_unzipper(ist1, num_items, items, LASzip::POINT_BY_POINT_RAW);
  LASunzipper* lasunzipper2 = make_unzipper(ist2, num_items, items, LASzip::POINT_BY_POINT_ARITHMETIC);
#ifdef LASZIP_HAVE_RANGECODER
  LASunzipper* lasunzipper3 = make_unzipper(ist3, num_items, items, LASzip::POINT_BY_POINT_RANGE);
#endif

  read_points(lasunzipper1, num_points, point_size, point_data, point);
  read_points(lasunzipper2, num_points, point_size, point_data, point);
#ifdef LASZIP_HAVE_RANGECODER
  read_points(lasunzipper3, num_points, point_size, point_data, point);
#endif

  delete lasunzipper1;
  delete lasunzipper2;
#ifdef LASZIP_HAVE_RANGECODER
  delete lasunzipper3;
#endif

  delete ist1;
  delete ist2;
  delete ist3;

  return 0;
}
