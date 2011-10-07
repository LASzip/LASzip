/*
===============================================================================

  FILE:  bytestreamin_istream.hpp
  
  CONTENTS:
      
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
  
     1 October 2011 -- added 64 bit file support in MSVC 6.0 at McCafe at Hbf Linz
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_IN_ISTREAM_H
#define BYTE_STREAM_IN_ISTREAM_H

#include "bytestreamin.hpp"

#ifdef LZ_WIN32_VC6
#include <fstream.h>
#else
#include <istream>
#include <fstream>
using namespace std;
#endif

class ByteStreamInIstream : public ByteStreamIn
{
public:
  ByteStreamInIstream(istream& stream);
/* read a single byte                                        */
  U32 getByte();
/* read an array of bytes                                    */
  BOOL getBytes(U8* bytes, const U32 num_bytes);
/* is the stream seekable (e.g. standard in is not)          */
  BOOL isSeekable() const;
/* get current position of stream                            */
  I64 tell() const;
/* seek to this position in the stream                       */
  BOOL seek(const I64 position);
/* seek to the end of the file                               */
  BOOL seekEnd(const I64 distance=0);
/* destructor                                                */
  ~ByteStreamInIstream(){};
protected:
  istream& stream;
};

class ByteStreamInIstreamLE : public ByteStreamInIstream
{
public:
  ByteStreamInIstreamLE(istream& stream);
/* read 16 bit low-endian field                              */
  BOOL get16bitsLE(U8* bytes);
/* read 32 bit low-endian field                              */
  BOOL get32bitsLE(U8* bytes);
/* read 64 bit low-endian field                              */
  BOOL get64bitsLE(U8* bytes);
/* read 16 bit big-endian field                              */
  BOOL get16bitsBE(U8* bytes);
/* read 32 bit big-endian field                              */
  BOOL get32bitsBE(U8* bytes);
/* read 64 bit big-endian field                              */
  BOOL get64bitsBE(U8* bytes);
private:
  U8 swapped[8];
};

class ByteStreamInIstreamBE : public ByteStreamInIstream
{
public:
  ByteStreamInIstreamBE(istream& stream);
/* read 16 bit low-endian field                              */
  BOOL get16bitsLE(U8* bytes);
/* read 32 bit low-endian field                              */
  BOOL get32bitsLE(U8* bytes);
/* read 64 bit low-endian field                              */
  BOOL get64bitsLE(U8* bytes);
/* read 16 bit big-endian field                              */
  BOOL get16bitsBE(U8* bytes);
/* read 32 bit big-endian field                              */
  BOOL get32bitsBE(U8* bytes);
/* read 64 bit big-endian field                              */
  BOOL get64bitsBE(U8* bytes);
private:
  U8 swapped[8];
};

inline ByteStreamInIstream::ByteStreamInIstream(istream& stream_param) :
  stream(stream_param)
{
}

inline U32 ByteStreamInIstream::getByte()
{
  int byte = stream.get();
  if (stream.eof())
  {
    throw EOF;
  }
  return (U32)byte;
}

inline BOOL ByteStreamInIstream::getBytes(U8* bytes, const U32 num_bytes)
{
  stream.read((char*)bytes, num_bytes);
  return stream.good();
}

inline BOOL ByteStreamInIstream::isSeekable() const
{
  return !!(static_cast<ifstream&>(stream));
}

inline I64 ByteStreamInIstream::tell() const
{
  return (I64)stream.tellg();
}

inline BOOL ByteStreamInIstream::seek(const I64 position)
{
  stream.seekg((streampos)position);
  return stream.good();
}

inline BOOL ByteStreamInIstream::seekEnd(const I64 distance)
{
  stream.seekg((streampos)-distance, ios::end);
  return stream.good();
}

inline ByteStreamInIstreamLE::ByteStreamInIstreamLE(istream& stream) : ByteStreamInIstream(stream)
{
}

inline BOOL ByteStreamInIstreamLE::get16bitsLE(U8* bytes)
{
  return getBytes(bytes, 2);
}

inline BOOL ByteStreamInIstreamLE::get32bitsLE(U8* bytes)
{
  return getBytes(bytes, 4);
}

inline BOOL ByteStreamInIstreamLE::get64bitsLE(U8* bytes)
{
  return getBytes(bytes, 8);
}

inline BOOL ByteStreamInIstreamLE::get16bitsBE(U8* bytes)
{
  if (getBytes(swapped, 2))
  {
    bytes[0] = swapped[1];
    bytes[1] = swapped[0];
    return TRUE;
  }
  return FALSE;
}

inline BOOL ByteStreamInIstreamLE::get32bitsBE(U8* bytes)
{
  if (getBytes(swapped, 4))
  {
    bytes[0] = swapped[3];
    bytes[1] = swapped[2];
    bytes[2] = swapped[1];
    bytes[3] = swapped[0];
    return TRUE;
  }
  return FALSE;
}

inline BOOL ByteStreamInIstreamLE::get64bitsBE(U8* bytes)
{
  if (getBytes(swapped, 8))
  {
    bytes[0] = swapped[7];
    bytes[1] = swapped[6];
    bytes[2] = swapped[5];
    bytes[3] = swapped[4];
    bytes[4] = swapped[3];
    bytes[5] = swapped[2];
    bytes[6] = swapped[1];
    bytes[7] = swapped[0];
    return TRUE;
  }
  return FALSE;
}

inline ByteStreamInIstreamBE::ByteStreamInIstreamBE(istream& stream) : ByteStreamInIstream(stream)
{
}

inline BOOL ByteStreamInIstreamBE::get16bitsLE(U8* bytes)
{
  if (getBytes(swapped, 2))
  {
    bytes[0] = swapped[1];
    bytes[1] = swapped[0];
    return TRUE;
  }
  return FALSE;
}

inline BOOL ByteStreamInIstreamBE::get32bitsLE(U8* bytes)
{
  if (getBytes(swapped, 4))
  {
    bytes[0] = swapped[3];
    bytes[1] = swapped[2];
    bytes[2] = swapped[1];
    bytes[3] = swapped[0];
    return TRUE;
  }
  return FALSE;
}

inline BOOL ByteStreamInIstreamBE::get64bitsLE(U8* bytes)
{
  if (getBytes(swapped, 8))
  {
    bytes[0] = swapped[7];
    bytes[1] = swapped[6];
    bytes[2] = swapped[5];
    bytes[3] = swapped[4];
    bytes[4] = swapped[3];
    bytes[5] = swapped[2];
    bytes[6] = swapped[1];
    bytes[7] = swapped[0];
    return TRUE;
  }
  return FALSE;
}

inline BOOL ByteStreamInIstreamBE::get16bitsBE(U8* bytes)
{
  return getBytes(bytes, 2);
}

inline BOOL ByteStreamInIstreamBE::get32bitsBE(U8* bytes)
{
  return getBytes(bytes, 4);
}

inline BOOL ByteStreamInIstreamBE::get64bitsBE(U8* bytes)
{
  return getBytes(bytes, 8);
}

#endif
