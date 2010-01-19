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

  FILE:  laswriter.cpp
  
  CONTENTS:
  
    see corresponding header file
  
  PROGRAMMERS:
  
    martin isenburg@cs.unc.edu
  
  COPYRIGHT:
  
    copyright (C) 2007-2008  martin isenburg@cs.unc.edu
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  
  CHANGE HISTORY:
  
    see corresponding header file
  
===============================================================================
*/
#include "laswriter.h"

#include "laspointwriter0raw.h"
#include "laspointwriter1raw.h"
#include "laspointwriter2raw.h"
#include "laspointwriter3raw.h"

#ifdef ENABLE_LAS_COMPRESSION
#include "laspointwriter0compressed.h"
#include "laspointwriter1compressed.h"
#include "laspointwriter2compressed.h"
#include "laspointwriter3compressed.h"
#include "laspointwriter0compressedarithmetic.h"
#include "laspointwriter1compressedarithmetic.h"
#include "laspointwriter2compressedarithmetic.h"
#include "laspointwriter3compressedarithmetic.h"
#endif // ENABLE_LAS_COMPRESSION

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include <stdlib.h>
#include <string.h>

bool LASwriter::open(FILE* file, LASheader* header, int compression)
{
  int i;

  if (file == 0)
  {
    fprintf(stderr,"ERROR: file pointer is zero\n");
    return false;
  }
  this->file = file;

#ifdef _WIN32
  if (file == stdout)
  {
    if(_setmode( _fileno( stdout ), _O_BINARY ) == -1 )
    {
      fprintf(stderr, "ERROR: cannot set stdout to binary (untranslated) mode\n");
    }
  }
#endif

  if (header == 0)
  {
    if (file == stdout)
    {
      fprintf(stderr,"ERROR: cannot use stdout without header\n");
      return false;
    }
    header = new LASheader();
    created_header = true;
  }
  this->header = header;

  // check header contents

  if (strncmp(header->file_signature, "LASF", 4) != 0)
  {
    fprintf(stderr,"ERROR: wrong file signature '%s'\n", header->file_signature);
    return false;
  }
  if ((header->version_major != 1) || ((header->version_minor != 0) && (header->version_minor != 1) && (header->version_minor != 2)))
  {
    fprintf(stderr,"WARNING: unknown version %d.%d (should be 1.0 or 1.1 or 1.2)\n", header->version_major, header->version_minor);
  }
  if (header->header_size != 227)
  {
    fprintf(stderr,"WARNING: header size is %d but should be 227\n", header->header_size);
  }
  if (header->offset_to_point_data < header->header_size)
  {
    fprintf(stderr,"ERROR: offset to point data %d is smaller than header size %d\n", header->offset_to_point_data, header->header_size);
    return false;
  }
  if (header->point_data_format == 0)
  {
    if (header->point_data_record_length != 20)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 20 for format 0\n", header->point_data_record_length);
    }
  }
  else if (header->point_data_format == 1)
  {
    if (header->point_data_record_length != 28)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 28 for format 1\n", header->point_data_record_length);
    }
  }
  else if (header->point_data_format == 2)
  {
    if (header->point_data_record_length != 26)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 26 for format 2\n", header->point_data_record_length);
    }
  }
  else if (header->point_data_format == 3)
  {
    if (header->point_data_record_length != 34)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 34 for format 3\n", header->point_data_record_length);
    }
  }
  else
  {
    fprintf(stderr,"WARNING: unknown point data format %d ... assuming format 0\n", header->point_data_format);
    header->point_data_format = 0;
  }
  if (header->x_scale_factor == 0 || header->y_scale_factor == 0 || header->z_scale_factor == 0)
  {
    fprintf(stderr,"WARNING: some scale factors are zero %g %g %g. those are set to 0.01.\n", header->x_scale_factor, header->y_scale_factor, header->z_scale_factor);
    if (header->x_scale_factor == 0) header->x_scale_factor = 0.01;
    if (header->y_scale_factor == 0) header->y_scale_factor = 0.01;
    if (header->z_scale_factor == 0) header->z_scale_factor = 0.01;
  }
  if (header->max_x < header->min_x || header->max_y < header->min_y || header->max_z < header->min_z)
  {
    fprintf(stderr,"WARNING: invalid bounding box [ %g %g %g / %g %g %g ]\n", header->min_x, header->min_y, header->min_z, header->max_x, header->max_y, header->max_z);
  }

  // create the right point writer in dependance on compression and point data format

  if (compression == LAS_COMPRESSION_RANGE)
  {
#ifdef ENABLE_LAS_COMPRESSION
    switch (header->point_data_format)
    {
    case 0:
      pointWriter = new LASpointWriter0compressed(file);
      break;
    case 1:
      pointWriter = new LASpointWriter1compressed(file);
      break;
    case 2:
      pointWriter = new LASpointWriter2compressed(file);
      break;
    case 3:
      pointWriter = new LASpointWriter3compressed(file);
      break;
    }
    // change the format to compressed
    header->point_data_format |= 128;
#else // ENABLE_LAS_COMPRESSION
    fprintf(stderr,"ERROR: this version of the laswriter does not support compression\n");
    return false;
#endif // ENABLE_LAS_COMPRESSION
  }
  else if (compression == LAS_COMPRESSION_ARITHMETIC)
  {
#ifdef ENABLE_LAS_COMPRESSION
    switch (header->point_data_format)
    {
    case 0:
      pointWriter = new LASpointWriter0compressedArithmetic(file);
      break;
    case 1:
      pointWriter = new LASpointWriter1compressedArithmetic(file);
      break;
    case 2:
      pointWriter = new LASpointWriter2compressedArithmetic(file);
      break;
    case 3:
      pointWriter = new LASpointWriter3compressedArithmetic(file);
      break;
    }
    // change the format to compressed
    header->point_data_format |= 64;
#else // ENABLE_LAS_COMPRESSION
    fprintf(stderr,"ERROR: this version of the laswriter does not support compression\n");
    return false;
#endif // ENABLE_LAS_COMPRESSION
  }
  else
  {
    switch (header->point_data_format)
    {
    case 0:
      pointWriter = new LASpointWriter0raw(file);
      break;
    case 1:
      pointWriter = new LASpointWriter1raw(file);
      break;
    case 2:
      pointWriter = new LASpointWriter2raw(file);
      break;
    case 3:
      pointWriter = new LASpointWriter3raw(file);
      break;
    }
  }

  // write header variable after variable (to avoid alignment issues)

  if (fwrite(&(header->file_signature), sizeof(char), 4, file) != 4)
  {
    fprintf(stderr,"ERROR: writing header->file_signature\n");
    return false;
  }
  if (fwrite(&(header->file_source_id), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->file_source_id\n");
    return false;
  }
  if (fwrite(&(header->global_encoding), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->global_encoding\n");
    return false;
  }
  if (fwrite(&(header->project_ID_GUID_data_1), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->project_ID_GUID_data_1\n");
    return false;
  }
  if (fwrite(&(header->project_ID_GUID_data_2), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->project_ID_GUID_data_2\n");
    return false;
  }
  if (fwrite(&(header->project_ID_GUID_data_3), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->project_ID_GUID_data_3\n");
    return false;
  }
  if (fwrite(&(header->project_ID_GUID_data_4), sizeof(char), 8, file) != 8)
  {
    fprintf(stderr,"ERROR: writing header->project_ID_GUID_data_4\n");
    return false;
  }
  if (fwrite(&(header->version_major), sizeof(char), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->version_major\n");
    return false;
  }
  if (fwrite(&(header->version_minor), sizeof(char), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->version_minor\n");
    return false;
  }
  if (fwrite(&(header->system_identifier), sizeof(char), 32, file) != 32)
  {
    fprintf(stderr,"ERROR: writing header->system_identifier\n");
    return false;
  }
  if (fwrite(&(header->generating_software), sizeof(char), 32, file) != 32)
  {
    fprintf(stderr,"ERROR: writing header->generating_software\n");
    return false;
  }
  if (fwrite(&(header->file_creation_day), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->file_creation_day\n");
    return false;
  }
  if (fwrite(&(header->file_creation_year), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->file_creation_year\n");
    return false;
  }
  if (fwrite(&(header->header_size), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->header_size\n");
    return false;
  }
  if (fwrite(&(header->offset_to_point_data), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->offset_to_point_data\n");
    return false;
  }
  if (fwrite(&(header->number_of_variable_length_records), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->number_of_variable_length_records\n");
    return false;
  }
  if (fwrite(&(header->point_data_format), sizeof(unsigned char), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->point_data_format\n");
    return false;
  }
  if (fwrite(&(header->point_data_record_length), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->point_data_record_length\n");
    return false;
  }
  if (fwrite(&(header->number_of_point_records), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->number_of_point_records\n");
    return false;
  }
  if (fwrite(&(header->number_of_points_by_return), sizeof(unsigned int), 5, file) != 5)
  {
    fprintf(stderr,"ERROR: writing header->number_of_points_by_return\n");
    return false;
  }
  if (fwrite(&(header->x_scale_factor), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->x_scale_factor\n");
    return false;
  }
  if (fwrite(&(header->y_scale_factor), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->y_scale_factor\n");
    return false;
  }
  if (fwrite(&(header->z_scale_factor), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->z_scale_factor\n");
    return false;
  }
  if (fwrite(&(header->x_offset), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->x_offset\n");
    return false;
  }
  if (fwrite(&(header->y_offset), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->y_offset\n");
    return false;
  }
  if (fwrite(&(header->z_offset), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->z_offset\n");
    return false;
  }
  if (fwrite(&(header->max_x), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->max_x\n");
    return false;
  }
  if (fwrite(&(header->min_x), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->min_x\n");
    return false;
  }
  if (fwrite(&(header->max_y), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->max_y\n");
    return false;
  }
  if (fwrite(&(header->min_y), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->min_y\n");
    return false;
  }
  if (fwrite(&(header->max_z), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->max_z\n");
    return false;
  }
  if (fwrite(&(header->min_z), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: writing header->min_z\n");
    return false;
  }

#ifdef ENABLE_LAS_COMPRESSION
  // change the format to compressed back to what it was
  header->point_data_format &= 63;
#endif // ENABLE_LAS_COMPRESSION

  // write any number of user-defined bytes that might have been added into the header

  if (header->user_data_in_header_size)
  {
    if (header->user_data_in_header == 0)
    {
      fprintf(stderr,"ERROR: there should be %d bytes of data in header->user_data_in_header\n", header->user_data_in_header_size);
      return false;
    }

    if (fwrite(header->user_data_in_header, sizeof(char), header->user_data_in_header_size, file) != (unsigned int)header->user_data_in_header_size)
    {
      fprintf(stderr,"ERROR: writing %d bytes of data from header->user_data_in_header\n", header->user_data_in_header_size);
      return false;
    }
  }

  // write variable length records variable after variable (to avoid alignment issues)

  for (i = 0; i < (int)header->number_of_variable_length_records; i++)
  {
    // check variable length records contents

    if (header->vlrs[i].reserved != 0xAABB)
    {
      fprintf(stderr,"WARNING: wrong header->vlrs[%d].reserved: %d != 0xAABB\n", i, header->vlrs[i].reserved);
    }

    // write variable length records variable after variable (to avoid alignment issues)

    if (fwrite(&(header->vlrs[i].reserved), sizeof(unsigned short), 1, file) != 1)
    {
      fprintf(stderr,"ERROR: writing header->vlrs[%d].reserved\n", i);
      return false;
    }
    if (fwrite(header->vlrs[i].user_id, sizeof(char), 16, file) != 16)
    {
      fprintf(stderr,"ERROR: writing header->vlrs[%d].user_id\n", i);
      return false;
    }
    if (fwrite(&(header->vlrs[i].record_id), sizeof(unsigned short), 1, file) != 1)
    {
      fprintf(stderr,"ERROR: writing header->vlrs[%d].record_id\n", i);
      return false;
    }
    if (fwrite(&(header->vlrs[i].record_length_after_header), sizeof(unsigned short), 1, file) != 1)
    {
      fprintf(stderr,"ERROR: writing header->vlrs[%d].record_length_after_header\n", i);
      return false;
    }
    if (fwrite(header->vlrs[i].description, sizeof(char), 32, file) != 32)
    {
      fprintf(stderr,"ERROR: writing header->vlrs[%d].description\n", i);
      return false;
    }

    // write the data following the header of the variable length record

    if (fwrite(header->vlrs[i].data, sizeof(char), header->vlrs[i].record_length_after_header, file) != header->vlrs[i].record_length_after_header)
    {
      fprintf(stderr,"ERROR: writing %d bytes of data from header->vlrs[%d].data\n", header->vlrs[i].record_length_after_header, i);
      return false;
    }
  }

  // write any number of user-defined bytes that might have been added after the header

  if (header->user_data_after_header_size)
  {
    if (header->user_data_after_header == 0)
    {
      fprintf(stderr,"ERROR: there should be %d bytes of data in header->user_data_after_header\n", header->user_data_after_header_size);
      return false;
    }

    if (fwrite(header->user_data_after_header, sizeof(char), header->user_data_after_header_size, file) != (unsigned int)header->user_data_after_header_size)
    {
      fprintf(stderr,"ERROR: writing %d bytes of data from header->user_data_after_header\n", header->user_data_after_header_size);
      return false;
    }
  }

  npoints = header->number_of_point_records;
  p_count = 0;

  return true;
}

bool LASwriter::write_point(LASpoint* point, double gps_time, unsigned short* rgb)
{
  p_count++;
  return pointWriter->write_point(point, gps_time, rgb);
}

bool LASwriter::write_point(double* coordinates)
{
  return write_point(coordinates[0], coordinates[1], coordinates[2]);
}

bool LASwriter::write_point(double x, double y, double z)
{
  LASpoint point;
  if (created_header)
  {
    if (p_count == 0) // then no header was provided
    {
      header->x_offset = 1000*((int)(x/1000));
      header->y_offset = 1000*((int)(y/1000));
      header->z_offset = 1000*((int)(z/1000));
      header->min_x = header->max_x = x;
      header->min_y = header->max_y = y;
      header->min_z = header->max_z = z;
    }
    else
    {
      if (x < header->min_x) header->min_x = x;
      else if (x > header->max_x) header->max_x = x;
      if (y < header->min_y) header->min_y = y;
      else if (y > header->max_y) header->max_y = y;
      if (z < header->min_z) header->min_z = z;
      else if (z > header->max_z) header->max_z = z;
    }
  }
  point.return_number = 1;
  point.number_of_returns_of_given_pulse = 1;
  if (x > header->x_offset)
    point.x = (int)((x-header->x_offset)/header->x_scale_factor + 0.5);
  else
    point.x = (int)((x-header->x_offset)/header->x_scale_factor - 0.5);
  if (y > header->y_offset)
    point.y = (int)((y-header->y_offset)/header->y_scale_factor + 0.5);
  else
    point.y = (int)((y-header->y_offset)/header->y_scale_factor - 0.5);
  if (z > header->z_offset)
    point.z = (int)((z-header->z_offset)/header->z_scale_factor + 0.5);
  else
    point.z = (int)((z-header->z_offset)/header->z_scale_factor - 0.5);
  return write_point(&point);
}

int LASwriter::close(bool update_header)
{
  if (pointWriter) 
  {
    delete pointWriter;
    pointWriter = 0;
  }
  if (created_header)
  {
    unsigned int number_of_point_records = (unsigned int)p_count;
    fseek(file, 107, SEEK_SET);
    fwrite(&number_of_point_records, sizeof(unsigned int), 1, file);
    fwrite(&number_of_point_records, sizeof(unsigned int), 1, file);
    fseek(file, 155, SEEK_SET);
    fwrite(&(header->x_offset), sizeof(double), 1, file);
    fwrite(&(header->y_offset), sizeof(double), 1, file);
    fwrite(&(header->z_offset), sizeof(double), 1, file);
    // enquantize and dequantize bounding box before writing it
    if (header->max_x > header->x_offset)
      header->max_x = header->x_offset + ((int)((header->max_x-header->x_offset)/header->x_scale_factor + 0.5)) * header->x_scale_factor;
    else
      header->max_x = header->x_offset + ((int)((header->max_x-header->x_offset)/header->x_scale_factor - 0.5)) * header->x_scale_factor;
    if (header->min_x > header->x_offset)
      header->min_x = header->x_offset + ((int)((header->min_x-header->x_offset)/header->x_scale_factor + 0.5)) * header->x_scale_factor;
    else
      header->min_x = header->x_offset + ((int)((header->min_x-header->x_offset)/header->x_scale_factor - 0.5)) * header->x_scale_factor;
    if (header->max_y > header->y_offset)
      header->max_y = header->y_offset + ((int)((header->max_y-header->y_offset)/header->y_scale_factor + 0.5)) * header->y_scale_factor;
    else
      header->max_y = header->y_offset + ((int)((header->max_y-header->y_offset)/header->y_scale_factor - 0.5)) * header->y_scale_factor;
    if (header->min_y > header->y_offset)
      header->min_y = header->y_offset + ((int)((header->min_y-header->y_offset)/header->y_scale_factor + 0.5)) * header->y_scale_factor;
    else
      header->min_y = header->y_offset + ((int)((header->min_y-header->y_offset)/header->y_scale_factor - 0.5)) * header->y_scale_factor;
    if (header->max_z > header->z_offset)
      header->max_z = header->z_offset + ((int)((header->max_z-header->z_offset)/header->z_scale_factor + 0.5)) * header->z_scale_factor;
    else
      header->max_z = header->z_offset + ((int)((header->max_z-header->z_offset)/header->z_scale_factor - 0.5)) * header->z_scale_factor;
    if (header->min_z > header->z_offset)
      header->min_z = header->z_offset + ((int)((header->min_z-header->z_offset)/header->z_scale_factor + 0.5)) * header->z_scale_factor;
    else
      header->min_z = header->z_offset + ((int)((header->min_z-header->z_offset)/header->z_scale_factor - 0.5)) * header->z_scale_factor;
    fwrite(&(header->max_x), sizeof(double), 1, file);
    fwrite(&(header->min_x), sizeof(double), 1, file);
    fwrite(&(header->max_y), sizeof(double), 1, file);
    fwrite(&(header->min_y), sizeof(double), 1, file);
    fwrite(&(header->max_z), sizeof(double), 1, file);
    fwrite(&(header->min_z), sizeof(double), 1, file);
    fseek(file, 0, SEEK_END);
    delete header;
    header = 0;
    npoints = p_count;
  }
  else if (p_count != npoints)
  {
    if (update_header)
    {
      if (file == stdout)
      {
	fprintf(stderr,"WARNING: cannot update header when using stdout\n");
      }
      else
      {
	fseek(file, 107, SEEK_SET);
	fwrite(&p_count, sizeof(unsigned int), 1, file);
	fseek(file, 0, SEEK_END);
      }
    }
    else if (npoints) fprintf(stderr,"WARNING: written %d points but expected %d points\n", p_count, npoints);
    npoints = p_count;
  }
  p_count = -1;
  int total_bytes = ftell(file);
  file = 0;
  return total_bytes;
}

LASwriter::LASwriter()
{
  npoints = -1;
  p_count = -1;
  file = 0;
  pointWriter = 0;
  header = 0;
  created_header = false;
}

LASwriter::~LASwriter()
{
}
