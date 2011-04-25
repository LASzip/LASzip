/*
===============================================================================

  FILE    laszippertest.cpp
  
  CONTENTS  
  
    This tool reads and writes point data in the LAS 1.X format compressed
    or uncompressed to test the laszipper and lasunzipper interfaces.

  PROGRAMMERS:

    martin.isenburg@gmail.com

  COPYRIGHT:

    (c) 2010-2011, Martin Isenburg, LASSO - tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    14 January 2011 -- mpg@flaxen.com adds randomness and commandline controls
    10 January 2011 -- licensing change for LGPL release and liblas integration
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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//---------------------------------------------------------------------------

static double taketime()
{
  return (double)(clock())/CLOCKS_PER_SEC;
}

//---------------------------------------------------------------------------
// abstractions for doing I/O, which support VC6 streams, modern streams, and FILE*

class OStream
{
public:
  OStream(bool use_iostream, const char* filename) :
    m_use_iostream(use_iostream),
    m_filename(filename),
    ofile(NULL),
    streamo(NULL)
  {
    if (m_use_iostream)
    {
#ifdef LZ_WIN32_VC6 
      ofb.open(filename, ios::out);
      ofb.setmode(filebuf::binary);
      streamo = new ostream(&ofb);
#else
      streamo = new ofstream();
      streamo->open(filename, std::ios::out | std::ios::binary );
#endif 
    }
    else
    {
      ofile = fopen(filename, "wb");
    }
  };

  ~OStream()
  {
    if (m_use_iostream)
    {
      delete streamo;
#ifdef LZ_WIN32_VC6 
      ofb.close();
#endif
    }
    else
    {
      if (ofile)
        fclose(ofile);
    }
  };

  bool m_use_iostream;
  const char* m_filename;
  FILE* ofile;
  filebuf ofb;
#ifdef LZ_WIN32_VC6 
  ostream* streamo;
#else
  ofstream* streamo;
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
    streami(NULL)
  {
    if (m_use_iostream)
    {
#ifdef LZ_WIN32_VC6 
      ifb.open(filename, ios::in);
      ifb.setmode(filebuf::binary);
      streami = new istream(&ifb);
#else
      streami = new ifstream();
      streami->open(filename, std::ios::in | std::ios::binary);
#endif 
    }
    else
    {
      ifile = fopen(filename, "rb");
    }
  };

  ~IStream()
  {
    if (m_use_iostream)
    {
      delete streami;
#ifdef LZ_WIN32_VC6 
      ifb.close();
#endif
    }
    else
    {
      if (ifile)
        fclose(ifile);
    }
  };

  bool m_use_iostream;
  const char* m_filename;
  FILE* ifile;
  filebuf ifb;
#ifdef LZ_WIN32_VC6 
  istream* streami;
#else
  ifstream* streami;
#endif
};


//---------------------------------------------------------------------------

#define NUM_ITEMS 5

class PointData
{
public:
  PointData()
  {
    num_items = NUM_ITEMS;

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

  ~PointData()
  {
    delete[] point;
    delete[] point_data;
    return;
  }

  unsigned int num_items;
  LASitem items[NUM_ITEMS];
  unsigned int point_size;
  unsigned char* point_data;
  unsigned char** point;
};


//---------------------------------------------------------------------------

class Settings
{
public:
  Settings(unsigned int num_pts, bool random, bool use_stream) :
    num_points(num_pts),
    use_random(random),
    use_iostream(use_stream)
  {
    logfile = fopen("zippertest.log","w");
    return;
  }

  ~Settings()
  {
    fclose(logfile);
    return;
  }

  unsigned num_points;
  bool use_random;
  unsigned int seed;
  bool use_iostream;
  FILE* logfile;
};

static Settings* settings = NULL; // singleton


//---------------------------------------------------------------------------

static void log(const char* format, ...)
{
  va_list args;

  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fflush(stderr);

  va_start(args, format);
  vfprintf(settings->logfile, format, args);
  va_end(args);  
  fflush(settings->logfile);

  return;
}

//---------------------------------------------------------------------------

static LASzipper* make_zipper(LASzip* laszip)
{
  LASzipper* zipper = new LASzipper();
  int stat = zipper->setup(laszip);
  if (stat != 0)
  {
    log("ERROR: could not make laszipper\n");
    exit(1);
  }
  return zipper;
}

//---------------------------------------------------------------------------

static void open_zipper(LASzipper* zipper, OStream* ost)
{
  int stat = 0;
  if (ost->m_use_iostream)
    stat = zipper->open(*ost->streamo);
  else
    stat = zipper->open(ost->ofile);
  if (stat != 0)
  {
    log("ERROR: could not open laszipper with %s\n", ost->m_filename);
    exit(1);
  }
}

//---------------------------------------------------------------------------

static LASunzipper* make_unzipper(LASzip* laszip)
{
  LASunzipper* unzipper = new LASunzipper();
  int stat = unzipper->setup(laszip);
  if (stat != 0)
  {
    log("ERROR: could not make lasunzipper\n");
    exit(1);
  }
  return unzipper;
}

//---------------------------------------------------------------------------

static void open_unzipper(LASunzipper* unzipper, IStream* ist)
{
  int stat = 0;
  if (ist->m_use_iostream)
    stat = unzipper->open(*ist->streami);
  else
    stat = unzipper->open(ist->ifile);
  if (stat != 0)
  {
    log("ERROR: could not open laszipper with %s\n", ist->m_filename);
    exit(1);
  }
}

//---------------------------------------------------------------------------

static void write_points(LASzipper* zipper, PointData& data)
{
  if (zipper==NULL) // range coder test
    return;

  double start_time, end_time;
  unsigned char c;
  unsigned int i,j;
  unsigned int num_bytes;

  // the two branches of this IF are the same, except for the use of a random number;
  // we keep the random case separate, so that we can get fastest timing tests w/o random data
  if (settings->use_random)
  {
    srand(settings->seed);
    start_time = taketime();
    c = rand() % 256;
    for (i = 0; i < settings->num_points; i++)
    {
      for (j = 0; j < data.point_size; j++)
      {
        data.point_data[j] = c;
        c = rand() % 256;
      }
      zipper->write(data.point);
    }
    num_bytes = zipper->close();
    end_time = taketime();
  }
  else
  {
    start_time = taketime();
    c = 0;
    for (i = 0; i < settings->num_points; i++)
    {
      for (j = 0; j < data.point_size; j++)
      {
        data.point_data[j] = c;
        c++;
      }
      zipper->write(data.point);
    }
    num_bytes = zipper->close();
    end_time = taketime();
  }

  log("laszipper wrote %u bytes in %g seconds\n", num_bytes, end_time-start_time);

  return;
}


//---------------------------------------------------------------------------

static void read_points(LASunzipper* unzipper, PointData& data)
{
  if (unzipper==NULL) // range coder test
    return;

  unsigned char c;
  unsigned int i,j;
  unsigned int num_errors, num_bytes;
  double start_time, end_time;

  start_time = taketime();
  num_errors = 0;

  if (settings->use_random)
  {
    srand(settings->seed);
    c = rand() % 256;
    for (i = 0; i < settings->num_points; i++)
    {
      unzipper->read(data.point);
      for (j = 0; j < data.point_size; j++)
      {
        if (data.point_data[j] != c)
        {
          log("%d %d %d != %d\n", i, j, data.point_data[j], c);
          num_errors++;
          if (num_errors > 20) break;
        }
        c = rand() % 256;
      }
      if (num_errors > 20) break;
    }
    num_bytes = unzipper->close();
    end_time = taketime();
  }
  else
  {
    c = 0;
    for (i = 0; i < settings->num_points; i++)
    {
      unzipper->read(data.point);
      for (j = 0; j < data.point_size; j++)
      {
        if (data.point_data[j] != c)
        {
          log("%d %d %d != %d\n", i, j, data.point_data[j], c);
          num_errors++;
          if (num_errors > 20) break;
        }
        c++;
      }
      if (num_errors > 20) break;
    }
    num_bytes = unzipper->close();
    end_time = taketime();
  }

  if (num_errors)
  {
    log("ERROR: with lasunzipper %d\n", num_errors);
    getc(stdin);
  }
  else
  {
    log("SUCCESS: lasunzipper read %u bytes in %g seconds\n", num_bytes, end_time-start_time);
  }

  return;
}


//---------------------------------------------------------------------------

static void run_test(const char* filename, PointData& data, unsigned short compressor, unsigned short requested_version=0, unsigned short chunk_size=0)
{
  LASzip laszip;
  laszip.setup(data.num_items, data.items, compressor);
  if (requested_version) laszip.request_version(requested_version);
  if (chunk_size) laszip.set_chunk_size(chunk_size);

  LASzipper* laszipper = make_zipper(&laszip);
  OStream* ost = new OStream(settings->use_iostream, filename);
  open_zipper(laszipper, ost);
  write_points(laszipper, data);
  delete laszipper;
  delete ost;

  LASunzipper* lasunzipper = make_unzipper(&laszip);
  IStream* ist = new IStream(settings->use_iostream, filename);
  open_unzipper(lasunzipper, ist);
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
  bool run_forever = false;
  bool use_random = false;
  unsigned int user_seed = (unsigned int)time(NULL);

  for (int i=1; i<argc; i++)
  {
    const char* p = argv[i];
    if (strcmp(p,"-n")==0)
    {
      ++i;
      num_points = atoi(argv[i]);
    }
    else if (strcmp(p,"-s")==0)
    {
      use_iostream = true;
    }
    else if (strcmp(p,"-f")==0)
    {
      run_forever = true;
    }
    else if (strcmp(p,"-r")==0)
    {
      use_random = true;
    }
    else if (strcmp(p,"-x")==0)
    {
      ++i;
      user_seed = atoi(argv[i]);
    }    
    else
    {
      fprintf(stderr, "Usage   ziptest [-n NUMBER] [-s] [-f] [-r]\n");
      fprintf(stderr, "   -n    number of points to process (default   100000)\n");
      fprintf(stderr, "   -s    use C++ stream I/O (default   use FILE* I/O)\n");
      fprintf(stderr, "   -f    run forever (default   run just one pass)\n");
      fprintf(stderr, "   -r    use random input data (default   use fixed data)\n");
      fprintf(stderr, "   -x    set random seed, instead of current time (default: use time)\n");
      exit(1);
    }
  }

  settings = new Settings(num_points, use_random, use_iostream);

  log("Settings:\n");
  log("  num_points=%u, use_iostream=%s, run_forever=%s, use_random=%s, user_seed=%d\n",
    num_points, use_iostream?"true":"false", run_forever?"true":"false", use_random?"true":"false", user_seed);

  settings->seed = user_seed;

  unsigned int run = 1;
  do
  {
  PointData data;

  // use a seed based on the current time
  if (run_forever && settings->use_random)
  {
    settings->seed = (unsigned int)time(NULL);
    log("Seed: %u\n", settings->seed);
  }

  run_test("test1.lax", data, LASZIP_COMPRESSOR_NONE);
  run_test("test2.lax", data, LASZIP_COMPRESSOR_DEFAULT);
  run_test("test3.lax", data, LASZIP_COMPRESSOR_DEFAULT, 2);
  run_test("test4.lax", data, LASZIP_COMPRESSOR_CHUNKED);
  run_test("test5.lax", data, LASZIP_COMPRESSOR_CHUNKED, 2);
  log("Finished %u runs\n\n", run);
  ++run;
  } while (run_forever);

  return 0;
}
