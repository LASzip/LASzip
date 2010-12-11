/******************************************************************************
 *
 * Project:  laszip - http://liblas.org -
 * Purpose:
 * Author:   Michael P. Gerlek (mpg@flaxen.com)
 *
 ******************************************************************************
 * Copyright (c) 2010, Michael P. Gerlek
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Licence as published
 * by the Free Software Foundation.
 *
 * See the COPYING file for more information.
 *
 ****************************************************************************/

#ifndef LASZIP_H
#define LASZIP_H

// These are the public interfaces exported out for use by by liblas.
// For now, they just wrap the "native" LASreader/LASwriter classes.

class LASreader;
class LASwriter;

class LasZipReader
{
public:
  LasZipReader(const char* file);
  ~LasZipReader();

private:
    LASreader* m_reader;
};

class LasZipWriter
{
public:
  LasZipWriter(const char* file);
  ~LasZipWriter();

private:
   LASwriter* m_writer;
};

#endif
