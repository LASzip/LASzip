/*
===============================================================================

  FILE:  bytestreamout_file.hpp
  
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
  
    10 January 2011 -- licensing change for LGPL release and liblas integration
    12 December 2010 -- created from ByteStreamOutFile after Howard got pushy (-;
  
===============================================================================
*/
#ifndef BYTE_STREAM_OUT_FILE_H
#define BYTE_STREAM_OUT_FILE_H

#include "bytestreamout.hpp"

#include <stdio.h>

class ByteStreamOutFile : public ByteStreamOut
{
public:
  ByteStreamOutFile(FILE* file);
/* write a single byte                                       */
  bool putByte(unsigned char byte);
/* write an array of bytes                                   */
  bool putBytes(const unsigned char* bytes, unsigned int num_bytes);
/* is the stream seekable (e.g. standard out is not)         */
  bool isSeekable() const;
/* save position in the stream for (forward) seeking later   */
  bool saveSeekPosition();
/* seek forward from last saved position (or start of file)  */
  bool seek(long offset);
/* seek to the end of the file                               */
  bool seekEnd();
/* returns how many bytes were written                       */
  unsigned int byteCount() const;
/* returns how many bytes were written                       */
  void resetCount();
/* destructor                                                */
  ~ByteStreamOutFile(){};
protected:
  FILE* file;
private:
  long start;
  long seek_position;
};

class ByteStreamOutFileLE : public ByteStreamOutFile
{
public:
  ByteStreamOutFileLE(FILE* file);
/* write 16 bit low-endian field                             */
  bool put16bitsLE(const unsigned char* bytes);
/* write 32 bit low-endian field                             */
  bool put32bitsLE(const unsigned char* bytes);
/* write 64 bit low-endian field                             */
  bool put64bitsLE(const unsigned char* bytes);
/* write 16 bit big-endian field                             */
  bool put16bitsBE(const unsigned char* bytes);
/* write 32 bit big-endian field                             */
  bool put32bitsBE(const unsigned char* bytes);
/* write 64 bit big-endian field                             */
  bool put64bitsBE(const unsigned char* bytes);
private:
  unsigned char swapped[8];
};

class ByteStreamOutFileBE : public ByteStreamOutFile
{
public:
  ByteStreamOutFileBE(FILE* file);
/* write 16 bit low-endian field                             */
  bool put16bitsLE(const unsigned char* bytes);
/* write 32 bit low-endian field                             */
  bool put32bitsLE(const unsigned char* bytes);
/* write 64 bit low-endian field                             */
  bool put64bitsLE(const unsigned char* bytes);
/* write 16 bit big-endian field                             */
  bool put16bitsBE(const unsigned char* bytes);
/* write 32 bit big-endian field                             */
  bool put32bitsBE(const unsigned char* bytes);
/* write 64 bit big-endian field                             */
  bool put64bitsBE(const unsigned char* bytes);
private:
  unsigned char swapped[8];
};

inline ByteStreamOutFile::ByteStreamOutFile(FILE* file)
{
  this->file = file;
  start = ftell(file);
  seek_position = ftell(file);
}

inline bool ByteStreamOutFile::putByte(unsigned char byte)
{
  return (fputc(byte, file) == byte);
}

inline bool ByteStreamOutFile::putBytes(const unsigned char* bytes, unsigned int num_bytes)
{
  return (fwrite(bytes, 1, num_bytes, file) == num_bytes);
}

inline bool ByteStreamOutFile::isSeekable() const
{
  return (file != stdout);
}

inline bool ByteStreamOutFile::saveSeekPosition()
{
  seek_position = ftell(file);
  return (seek_position != -1L);
}

inline bool ByteStreamOutFile::seek(long offset)
{
  return !(fseek(file, seek_position+offset, SEEK_SET));
}

inline bool ByteStreamOutFile::seekEnd()
{
  return !(fseek(file, 0, SEEK_END));
}

inline unsigned int ByteStreamOutFile::byteCount() const
{
  return ftell(file)-start;
}

inline void ByteStreamOutFile::resetCount()
{
  start = ftell(file);
}

inline ByteStreamOutFileLE::ByteStreamOutFileLE(FILE* file) : ByteStreamOutFile(file)
{
}

inline bool ByteStreamOutFileLE::put16bitsLE(const unsigned char* bytes)
{
  return putBytes(bytes, 2);
}

inline bool ByteStreamOutFileLE::put32bitsLE(const unsigned char* bytes)
{
  return putBytes(bytes, 4);
}

inline bool ByteStreamOutFileLE::put64bitsLE(const unsigned char* bytes)
{
  return putBytes(bytes, 8);
}

inline bool ByteStreamOutFileLE::put16bitsBE(const unsigned char* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline bool ByteStreamOutFileLE::put32bitsBE(const unsigned char* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline bool ByteStreamOutFileLE::put64bitsBE(const unsigned char* bytes)
{
  swapped[0] = bytes[7];
  swapped[1] = bytes[6];
  swapped[2] = bytes[5];
  swapped[3] = bytes[4];
  swapped[4] = bytes[3];
  swapped[5] = bytes[2];
  swapped[6] = bytes[1];
  swapped[7] = bytes[0];
  return putBytes(swapped, 8);
}

inline ByteStreamOutFileBE::ByteStreamOutFileBE(FILE* file) : ByteStreamOutFile(file)
{
}

inline bool ByteStreamOutFileBE::put16bitsLE(const unsigned char* bytes)
{
  swapped[0] = bytes[1];
  swapped[1] = bytes[0];
  return putBytes(swapped, 2);
}

inline bool ByteStreamOutFileBE::put32bitsLE(const unsigned char* bytes)
{
  swapped[0] = bytes[3];
  swapped[1] = bytes[2];
  swapped[2] = bytes[1];
  swapped[3] = bytes[0];
  return putBytes(swapped, 4);
}

inline bool ByteStreamOutFileBE::put64bitsLE(const unsigned char* bytes)
{
  swapped[0] = bytes[7];
  swapped[1] = bytes[6];
  swapped[2] = bytes[5];
  swapped[3] = bytes[4];
  swapped[4] = bytes[3];
  swapped[5] = bytes[2];
  swapped[6] = bytes[1];
  swapped[7] = bytes[0];
  return putBytes(swapped, 8);
}

inline bool ByteStreamOutFileBE::put16bitsBE(const unsigned char* bytes)
{
  return putBytes(bytes, 2);
}

inline bool ByteStreamOutFileBE::put32bitsBE(const unsigned char* bytes)
{
  return putBytes(bytes, 4);
}

inline bool ByteStreamOutFileBE::put64bitsBE(const unsigned char* bytes)
{
  return putBytes(bytes, 8);
}

#endif
