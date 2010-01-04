/******************************************************************************
 *
 * Project:  laszip - http://liblas.org - 
 * Purpose:  
 * Author:   Martin Isenburg
 *           martin.isenburg at gmail.com
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

typedef unsigned int    U32;
typedef unsigned short  U16;
typedef unsigned char   U8;

#if defined(WIN32)            // 64 byte integer under Windows 
typedef unsigned __int64   U64;
typedef __int64            I64;
#else                          // 64 byte integer elsewhere ... 
typedef unsigned long long U64;
typedef long long          I64;
#endif 
typedef int              I32;
typedef short            I16;
typedef signed char      I8;

typedef double          F64;
typedef float            F32;

typedef int              BOOL;
typedef char            CHAR;

#define F32_MAX    +1.0e+30f
#define F32_MIN    -1.0e+30f

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
