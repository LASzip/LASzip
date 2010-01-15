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

/*
===============================================================================

  FILE:  integercompressor_context.cpp
  
  CONTENTS:

    see corresponding header file

  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2005  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/

#include "integercompressor_context.h"

#define COMPRESS_ONLY_K
#undef COMPRESS_ONLY_K

#define CREATE_HISTOGRAMS
#undef CREATE_HISTOGRAMS

#include <stdlib.h>
#include <assert.h>

#ifdef CREATE_HISTOGRAMS
#include <math.h>
#endif

IntegerCompressorContext::IntegerCompressorContext()
{
  bits = 16;
  range = 0;

  k = 0;

  re = 0;
  rd = 0;

  rmBits = 0;

  rmCorrector = 0;
}

IntegerCompressorContext::~IntegerCompressorContext()
{
  U32 i;
  if (rmBits)
  {
    for (i = 0; i < contexts; i++)
    {
      delete rmBits[i];
    }
    free(rmBits);
  }
  if (rmCorrector)
  {
    for (i = 0; i <= corr_bits; i++)
    {
      if (rmCorrector[i])
      {
        delete rmCorrector[i];
      }
    }
    free(rmCorrector);
  }
}

void IntegerCompressorContext::SetupCompressor(RangeEncoder* re, U32 contexts, U32 bits_high)
{
  U32 i;

  this->re = re;
  this->contexts = contexts;
  this->bits_high = bits_high;

  // the corrector's significant bits and range  

  if (range)
  {
    corr_bits = 0;
    corr_range = range;
    while (range)
    {
      range = range >> 1;
      corr_bits++;
    }
    if (corr_range == (1u << (corr_bits-1)))
    {
      corr_bits--;
    }
    // the corrector must fall into this interval
    corr_min = -((I32)(corr_range/2));
    corr_max = corr_min + corr_range - 1;
  }
  else if (bits && bits < 32)
  {
    corr_bits = bits;
    corr_range = 1 << bits;
    // the corrector must fall into this interval
    corr_min = -((I32)(corr_range/2));
    corr_max = corr_min + corr_range - 1;
  }
  else
  {
    corr_bits = 32;
    corr_range = 0;
    // the corrector must fall into this interval
    corr_min = I32_MIN;
    corr_max = I32_MAX;
  }

  // allocate the arithmetic range tables 

  rmBits = (RangeModel**)malloc(sizeof(RangeModel*) * (contexts));
  for (i = 0; i < contexts; i++)
  {
    rmBits[i] = new RangeModel(corr_bits+1,0,1,512,16);
  }

#if defined(COMPRESS_ONLY_K)
  rmCorrector = 0;
#else
  rmCorrector = (RangeModel**)malloc(sizeof(RangeModel*) * (corr_bits+1));
  for (i = 0; i <= corr_bits; i++)
  {
    rmCorrector[i] = 0;
  }
#endif

#ifdef CREATE_HISTOGRAMS
  corr_histogram = (int**)malloc(sizeof(int*) * (corr_bits+1));
  for (int k = 0; k <= corr_bits; k++)
  {
    corr_histogram[k] = (int*)malloc(sizeof(int) * ((1<<k)+1));
    for (int c = 0; c <= (1<<k); c++)
    {
      corr_histogram[k][c] = 0;
    }
  }  
#endif
}

void IntegerCompressorContext::FinishCompressor()
{
  U32 i;
  if (rmBits)
  {
    for (i = 0; i < contexts; i++)
    {
      delete rmBits[i];
    }
    free(rmBits);
    rmBits = 0;
  }
  if (rmCorrector)
  {
    for (i = 0; i <= corr_bits; i++)
    {
      if (rmCorrector[i])
      {
        delete rmCorrector[i];
      }
    }
    free(rmCorrector);
    rmCorrector = 0;
  }

#ifdef CREATE_HISTOGRAMS
  int total_number = 0;
  double total_entropy = 0.0f;
  double total_raw = 0.0f;
  for (int k = 0; k <= corr_bits; k++)
  {
    int number = 0;
    int different = 0;
    for (int c = 0; c <= (1<<k); c++)
    {
      number += corr_histogram[k][c];
    }
    double prob,entropy = 0.0f;
    for (c = 0; c <= (1<<k); c++)
    {
      if (corr_histogram[k][c])
      {
        different++;
        prob = (double)corr_histogram[k][c]/(double)number;
        entropy -= log(prob)*prob/log(2.0);
      }
    }
    fprintf(stderr, "k: %d number: %d different: %d entropy: %lg raw: %1.1f\n",k,number,different,entropy, (float)(k?k:1));
    total_number += number;
    total_entropy += (entropy*number);
    total_raw += ((k?k:1)*number);
  }  
  fprintf(stderr, "TOTAL: number: %d entropy: %lg raw: %lg\n",total_number,total_entropy/total_number,total_raw/total_number);
#endif
}

void IntegerCompressorContext::SetupDecompressor(RangeDecoder* rd, U32 contexts, U32 bits_high)
{
  U32 i;

  this->rd = rd;
  this->contexts = contexts;
  this->bits_high = bits_high;

  // the corrector's significant bits and range  

  if (range)
  {
    corr_bits = 0;
    corr_range = range;
    while (range)
    {
      range = range >> 1;
      corr_bits++;
    }
    if (corr_range == (1u << (corr_bits-1)))
    {
      corr_bits--;
    }
    // the corrector must fall into this interval
    corr_min = -((I32)(corr_range/2));
    corr_max = corr_min + corr_range - 1;
  }
  else if (bits && bits < 32)
  {
    corr_bits = bits;
    corr_range = 1u << bits;
    // the corrector must fall into this interval
    corr_min = -((I32)(corr_range/2));
    corr_max = corr_min + corr_range - 1;
  }
  else
  {
    corr_bits = 32;
    corr_range = 0;
    // the corrector must fall into this interval
    corr_min = I32_MIN;
    corr_max = I32_MAX;
  }

  // allocate the arithmetic range tables 

  rmBits = (RangeModel**)malloc(sizeof(RangeModel*) * (contexts));
  for (i = 0; i < contexts; i++)
  {
    rmBits[i] = new RangeModel(corr_bits+1,0,0,512,16);
  }

#if defined(COMPRESS_ONLY_K)
  rmCorrector = 0;
#else
  rmCorrector = (RangeModel**)malloc(sizeof(RangeModel*) * (corr_bits+1));
  for (i = 0; i <= corr_bits; i++)
  {
    rmCorrector[i] = 0;
  }
#endif
}

void IntegerCompressorContext::FinishDecompressor()
{
  U32 i;
  if (rmBits)
  {
    for (i = 0; i < contexts; i++)
    {
      delete rmBits[i];
    }
    free(rmBits);
    rmBits = 0;
  }

  if (rmCorrector)
  {
    for (i = 0; i <= corr_bits; i++)
    {
      if (rmCorrector[i])
      {
        delete rmCorrector[i];
      }
    }
    free(rmCorrector);
    rmCorrector = 0;
  }
}

//-----------------------------------------------------------------------------
// writeCorrector:
//-----------------------------------------------------------------------------
void IntegerCompressorContext::writeCorrector(I32 c, RangeModel* rmBits)
{
  I32 c1;

  // find the tighest interval [ - (2^k - 1)  ...  + (2^k) ] that contains c

  k = 0;

  // do this by checking the absolute value of c (adjusted for the case that c is 2^k)

  c1 = (c <= 0 ? -c : c-1);

  // this loop could be replaced with more efficient code

  while (c1)
  {
    c1 = c1 >> 1;
    k = k + 1;
  }

  // the number k is between 0 and corr_bits and describes the interval the corrector falls into
  // we can compress the exact location of c within this interval using k bits

  re->encode(rmBits, k);

//  printf("k%d ", k);

#ifdef COMPRESS_ONLY_K
  if (k) // then c is either smaller than 0 or bigger than 1
  {
    assert((c != 0) && (c != 1));
    // translate the corrector c into the k-bit interval [ 0 ... 2^k - 1 ]
    if (c < 0) // then c is in the interval [ - (2^k - 1)  ...  - (2^(k-1)) ]
    {
      // so we translate c into the interval [ 0 ...  + 2^(k-1) - 1 ] by adding (2^k - 1)
      re->encode((1<<k), c + ((1<<k) - 1));
#ifdef CREATE_HISTOGRAMS
      corr_histogram[k][c + ((1<<k) - 1)]++;
#endif
//      printf("c%d ", c + ((1<<k) - 1));
    }
    else // then c is in the interval [ 2^(k-1) + 1  ...  2^k ]
    {
      // so we translate c into the interval [ 2^(k-1) ...  + 2^k - 1 ] by subtracting 1
      re->encode((1<<k), c - 1);
#ifdef CREATE_HISTOGRAMS
      corr_histogram[k][c - 1]++;
#endif
//      printf("c%d ", c - 1);
    }
  }
  else // then c is 0 or 1
  {
    assert((c == 0) || (c == 1));
    re->encode(2,c);
#ifdef CREATE_HISTOGRAMS
    corr_histogram[0][c]++;
#endif
//    printf("cn%d ",c);
  }
#else // COMPRESS_ONLY_K
  if (k) // then c is either smaller than 0 or bigger than 1
  {
    assert((c != 0) && (c != 1));
    // translate the corrector c into the k-bit interval [ 0 ... 2^k - 1 ]
    if (c < 0) // then c is in the interval [ - (2^k - 1)  ...  - (2^(k-1)) ]
    {
      // so we translate c into the interval [ 0 ...  + 2^(k-1) - 1 ] by adding (2^k - 1)
      c += ((1<<k) - 1);
    }
    else // then c is in the interval [ 2^(k-1) + 1  ...  2^k ]
    {
      // so we translate c into the interval [ 2^(k-1) ...  + 2^k - 1 ] by subtracting 1
      c -= 1;
    }

    if (k <= bits_high) // for small k we code the interval in one step
    {
      // allocate a new range table if necessary
      if (rmCorrector[k] == 0)
      {
        rmCorrector[k] = new RangeModel((1<<k),0,1,2048,16);
      }
      // compress c with the range coder
      re->encode(rmCorrector[k], c);
    }
    else // for larger k we need to code the interval in two steps
    {
      // figure out how many lower bits there are 
      int k1 = k-bits_high;
      // c1 represents the lowest k-bits_high+1 bits
      c1 = c & ((1<<k1) - 1);
      // c represents the highest bits_high bits
      c = c >> k1;
      // allocate a new range table if necessary
      if (rmCorrector[k] == 0)
      {
        rmCorrector[k] = new RangeModel(1<<bits_high,0,1,2048,16);
      }
      // compress the higher bits using a context table
      re->encode(rmCorrector[k], c);
      // store the lower k1 bits raw
      re->writeBits(k1, c1);
    }
  }
  else // then c is 0 or 1
  {
    assert((c == 0) || (c == 1));
    // allocate a new range table if necessary
    if (rmCorrector[0] == 0)
    {
      rmCorrector[0] = new RangeModel(2,0,1,2048,16);
    }
    re->encode(rmCorrector[0],c);
  }
#endif // COMPRESS_ONLY_K
}

void IntegerCompressorContext::Compress(I32 pred, I32 real, U32 context)
{
  // the corrector will be within the interval [ - (corr_range - 1)  ...  + (corr_range - 1) ]
  int corr = real - pred;
  // we fold the corrector into the interval [ corr_min  ...  corr_max ]
  if (corr < corr_min) corr += corr_range;
  else if (corr > corr_max) corr -= corr_range;
  writeCorrector(corr, rmBits[context]);
}

I32 IntegerCompressorContext::readCorrector(RangeModel* rmBits)
{
  I32 c;

  // decode within which interval the corrector is falling

  k = rd->decode(rmBits);

//  printf("k%d ", k);

  // decode the exact location of the corrector within the interval

#ifdef COMPRESS_ONLY_K
  if (k) // then c is either smaller than 0 or bigger than 1
  {
    c = rd->decode(1<<k);

//    printf("c%d ", c);

    if (c >= (1<<(k-1))) // if c is in the interval [ 2^(k-1)  ...  + 2^k - 1 ]
    {
      // so we translate c back into the interval [ 2^(k-1) + 1  ...  2^k ] by adding 1 
      c += 1;
    }
    else // otherwise c is in the interval [ 0 ...  + 2^(k-1) - 1 ]
    {
      // so we translate c back into the interval [ - (2^k - 1)  ...  - (2^(k-1)) ] by subtracting (2^k - 1)
      c -= ((1<<k) - 1);
    }
  }
  else // then c is either 0 or 1
  {
    c = rd->decode(2);
//    printf("cn%d ", c);
  }
#else // COMPRESS_ONLY_K
  if (k) // then c is either smaller than 0 or bigger than 1
  {
    if (k <= bits_high)
    {
      // for small k we can do this in one step

      // allocate a new range table if necessary
      if (rmCorrector[k] == 0)
      {
        rmCorrector[k] = new RangeModel((1<<k),0,0,2048,16);
      }
      // decompress c with the range coder
      c = rd->decode(rmCorrector[k]);
    }
    else
    {
      // for larger k we need to do this in two steps
      int k1 = k-bits_high;

      // allocate a new range table if necessary
      if (rmCorrector[k] == 0)
      {
        rmCorrector[k] = new RangeModel(1<<bits_high,0,0,2048,16);
      }
      // decompress higher bits with table
      c = rd->decode(rmCorrector[k]);
      // read lower bits raw
      int c1 = rd->readBits(k1);
      // put the corrector back together
      c = (c << k1) | c1;
    }

    // translate c back into its correct interval
    if (c >= (1<<(k-1))) // if c is in the interval [ 2^(k-1)  ...  + 2^k - 1 ]
    {
      // so we translate c back into the interval [ 2^(k-1) + 1  ...  2^k ] by adding 1 
      c += 1;
    }
    else // otherwise c is in the interval [ 0 ...  + 2^(k-1) - 1 ]
    {
      // so we translate c back into the interval [ - (2^k - 1)  ...  - (2^(k-1)) ] by subtracting (2^k - 1)
      c -= ((1<<k) - 1);
    }
  }
  else // then c is either 0 or 1
  {
    // allocate a new range table if necessary
    if (rmCorrector[0] == 0)
    {
      rmCorrector[0] = new RangeModel(2,0,0,2048,16);
    }
    c = rd->decode(rmCorrector[0]);
  }
#endif // COMPRESS_ONLY_K

  return c;
}

I32 IntegerCompressorContext::Decompress(I32 pred, U32 context)
{
  I32 real = pred + readCorrector(rmBits[context]);
  if (real < 0) real += corr_range;
  else if ((U32)(real) >= corr_range) real -= corr_range;
  return real;
}
