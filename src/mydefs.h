/******************************************************************************
 *
 * Project:  laszip - http://liblas.org -
 * Purpose:
 * Author:   Martin Isenburg
 *           isenburg at cs.unc.edu
 *
 ******************************************************************************
 * Copyright (c) 2009, Martin Isenburg
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
typedef signed char        I8;

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
#define FALSE    0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef NULL
#define NULL    0
#endif

#endif
