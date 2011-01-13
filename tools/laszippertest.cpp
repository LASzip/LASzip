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


//---------------------------------------------------------------------------

static double taketime()
{
  return (double)(clock())/CLOCKS_PER_SEC;
}


//---------------------------------------------------------------------------
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


//---------------------------------------------------------------------------

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


//---------------------------------------------------------------------------

class Data
{
public:
    Data(unsigned int num_pts, bool random) :
      num_points(num_pts),
      use_random(random)
    {
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

        unsigned int i;

        // compute the point size
        point_size = 0;
        for (i = 0; i < num_items; i++) point_size += items[i].size;

        // create the point data
        unsigned int point_offset = 0;
        point = new unsigned char*[num_items];
        point_data = new unsigned char[point_size];
        for (i = 0; i < num_items; i++)
        {
            point[i] = &(point_data[point_offset]);
            point_offset += items[i].size;
        }

        return;
    }

    ~Data()
    {
        delete[] point;
        delete[] point_data;
        return;
    }

    static const unsigned int num_items = 5;
    LASitem items[num_items];
    unsigned int point_size;
    unsigned char* point_data;
    unsigned char** point;
    unsigned num_points;
    bool use_random;
};


//---------------------------------------------------------------------------

static LASzipper* make_zipper(OStream* ost, Data& data, LASzip::Algorithm alg)
{
#ifndef LASZIP_HAVE_RANGECODER
    if (alg == LASzip::POINT_BY_POINT_RANGE)
    {
        fprintf(stderr, "(skipping range encoder test)\n");
        return NULL;
    }
#endif

    LASzipper* zipper = new LASzipper();

    int stat = 0;
    if (ost->m_use_iostream)
        stat = zipper->open(*ost->ostream, data.num_items, data.items, alg);
    else
        stat = zipper->open(ost->ofile, data.num_items, data.items, alg);

    if (stat != 0)
    {
      fprintf(stderr, "ERROR: could not open laszipper with %s\n", ost->m_filename);
      exit(1);
    }

    return zipper;
}


//---------------------------------------------------------------------------

static LASunzipper* make_unzipper(IStream* ist, Data& data, LASzip::Algorithm alg)
{
#ifndef LASZIP_HAVE_RANGECODER
    if (alg == LASzip::POINT_BY_POINT_RANGE)
    {
        return NULL;
    }
#endif

    LASunzipper* unzipper = new LASunzipper();

    int stat = 0;
    if (ist->m_use_iostream)
        stat = unzipper->open(*ist->istream, data.num_items, data.items, alg);
    else
        stat = unzipper->open(ist->ifile, data.num_items, data.items, alg);

    if (stat != 0)
    {
      fprintf(stderr, "ERROR: could not open lasunzipper with %s\n", ist->m_filename);
      exit(1);
    }

    return unzipper;
}


//---------------------------------------------------------------------------

static void write_points(LASzipper* zipper, Data& data)
{
    if (zipper==NULL) // range coder test
        return;

    double start_time, end_time;
    unsigned char c;
    unsigned int i,j;

    start_time = taketime();
    c = 0;
    for (i = 0; i < data.num_points; i++)
    {
        for (j = 0; j < data.point_size; j++)
        {
            data.point_data[j] = c;
            c++;
        }
        zipper->write(data.point);
    }

    unsigned int num_bytes = zipper->close();
    end_time = taketime();
    fprintf(stderr, "laszipper wrote %d bytes in %g seconds\n", num_bytes, end_time-start_time);

    return;
}


//---------------------------------------------------------------------------

static void read_points(LASunzipper* unzipper, Data& data)
{
    if (unzipper==NULL) // range coder test
        return;

    unsigned char c;
    unsigned int i,j;
    unsigned int num_errors, num_bytes;
    double start_time, end_time;

    start_time = taketime();
    num_errors = 0;

    c = 0;
    for (i = 0; i < data.num_points; i++)
    {
        unzipper->read(data.point);
        for (j = 0; j < data.point_size; j++)
        {
            if (data.point_data[j] != c)
            {
                fprintf(stderr, "%d %d %d != %d\n", i, j, data.point_data[j], c);
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


//---------------------------------------------------------------------------

static void run_test(bool use_iostream, const char* filename, Data& data, LASzip::Algorithm alg)
{
  OStream* ost = new OStream(use_iostream, filename);
  LASzipper* laszipper = make_zipper(ost, data, alg);
  write_points(laszipper, data);
  delete laszipper;
  delete ost;

  IStream* ist = new IStream(use_iostream, filename);
  LASunzipper* lasunzipper = make_unzipper(ist, data, alg);
  read_points(lasunzipper, data);
  delete lasunzipper;
  delete ist;

  return;
}


//---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  unsigned int num_points = 100000;
  bool use_iostream = false;
  bool forever = false;
  bool use_random = true;

  do
  {
    Data data(num_points, use_random);

    run_test(use_iostream, "test1.lax", data, LASzip::POINT_BY_POINT_RAW);
    run_test(use_iostream, "test2.lax", data, LASzip::POINT_BY_POINT_ARITHMETIC);
    run_test(use_iostream, "test3.lax", data, LASzip::POINT_BY_POINT_RANGE);
  } while (forever);

  return 0;
}
