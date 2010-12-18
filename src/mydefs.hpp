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

#ifndef MYDEFS_H
#define MYDEFS_H

typedef int                I32;
typedef short              I16;
typedef char               I8;

typedef unsigned int       U32;
typedef unsigned short     U16;
typedef unsigned char      U8;

#if defined(WIN32)            // 64 byte integer under Windows 
typedef unsigned __int64   U64;
typedef __int64            I64;
#else                          // 64 byte integer elsewhere ... 
typedef unsigned long long U64;
typedef long long          I64;
#endif

typedef float              F32;
typedef double             F64;

typedef bool               BOOL;

typedef union U32F32 { U32 u32; F32 f32; } U32F32;
typedef union U64F64 { U64 u64; F64 f64; } U64F64;
typedef union I32F32 { I32 i32; F32 f32; } I32F32;
typedef union I64F64 { I64 i64; F64 f64; } I64F64;

#define F32_MAX            +1.0e+30f
#define F32_MIN            -1.0e+30f

#define U8_MAX             0xFF // 255
#define U8_MAX_PLUS_ONE    0x0100 // 256

#define U16_MAX            0xFFFF // 65535
#define U16_MAX_PLUS_ONE   0x00010000 // 65536

#define U32_MAX            0xFFFFFFFF // 4294967295
#if defined(WIN32)            // 64 byte unsigned int constant under Windows 
#define U32_MAX_PLUS_ONE   0x0000000100000000 // 4294967296
#else                         // 64 byte unsigned int constant elsewhere ... 
#define U32_MAX_PLUS_ONE   0x0000000100000000ull // 4294967296
#endif

#define I8_MIN             0x80 // -128
#define I8_MAX             0x7F // 127

#define I16_MIN            0x8000 // -32768
#define I16_MAX            0x7FFF // 32767

#define I32_MIN            0x80000000 // -2147483648
#define I32_MAX            0x7FFFFFFF //  2147483647

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef NULL
#define NULL    0
#endif

inline BOOL IS_LITTLE_ENDIAN()
{
  U32 i = 1;
  if (*((U8*)&i) == 1)
    return TRUE;
  else
    return FALSE;
}

inline void ENDIAN_SWAP_16(U8* field)
{
  U8 help = field[0];
  field[0] = field[1];
  field[1] = help;
}

inline void ENDIAN_SWAP_32(U8* field)
{
  U8 help;
  help = field[0];
  field[0] = field[3];
  field[3] = help;
  help = field[1];
  field[1] = field[2];
  field[2] = help;
}

inline void ENDIAN_SWAP_64(U8* field)
{
  U8 help;
  help = field[0];
  field[0] = field[7];
  field[7] = help;
  help = field[1];
  field[1] = field[6];
  field[6] = help;
  help = field[2];
  field[2] = field[5];
  field[5] = help;
  help = field[3];
  field[3] = field[4];
  field[4] = help;
}

inline void ENDIAN_SWAP_16(U8* from, U8* to)
{
  to[0] = from[1];
  to[1] = from[0];
}

inline void ENDIAN_SWAP_32(U8* from, U8* to)
{
  to[0] = from[3];
  to[1] = from[2];
  to[2] = from[1];
  to[3] = from[0];
}

inline void ENDIAN_SWAP_64(U8* from, U8* to)
{
  to[0] = from[7];
  to[1] = from[6];
  to[2] = from[5];
  to[3] = from[4];
  to[4] = from[3];
  to[5] = from[2];
  to[6] = from[1];
  to[7] = from[0];
}

#endif
