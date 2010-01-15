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

  FILE:  lasreader.cpp
  
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
#include "lasreader.h"

#define ENABLE_LAS_COMPRESSION_SUPPORT
//#undef ENABLE_LAS_COMPRESSION_SUPPORT

#include "laspointreader0raw.h"
#include "laspointreader1raw.h"
#include "laspointreader2raw.h"
#include "laspointreader3raw.h"

#ifdef ENABLE_LAS_COMPRESSION_SUPPORT
#include "laspointreader0compressed.h"
#include "laspointreader1compressed.h"
#include "laspointreader2compressed.h"
#include "laspointreader3compressed.h"
#endif // ENABLE_LAS_COMPRESSION_SUPPORT

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#include <stdlib.h>
#include <string.h>

bool LASreader::open(FILE* file, bool skip_all_headers)
{
  int i;

  if (file == 0)
  {
    fprintf(stderr,"ERROR: file pointer is zero\n");
    return false;
  }

#ifdef _WIN32
  if (file == stdin)
  {
    if(_setmode( _fileno( stdin ), _O_BINARY ) == -1 )
    {
      fprintf(stderr, "ERROR: cannot set stdin to binary (untranslated) mode\n");
    }
  }
#endif

  this->file = file;

  // clean the header
  header.clean();

  // read the header variable after variable (to avoid alignment issues)

  if (fread(&(header.file_signature), sizeof(char), 4, file) != 4)
  {
    fprintf(stderr,"ERROR: reading header.file_signature\n");
    return false;
  }
  if (fread(&(header.file_source_id), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.file_source_id\n");
    return false;
  }
  if (fread(&(header.global_encoding), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.global_encoding\n");
    return false;
  }
  if (fread(&(header.project_ID_GUID_data_1), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_1\n");
    return false;
  }
  if (fread(&(header.project_ID_GUID_data_2), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_2\n");
    return false;
  }
  if (fread(&(header.project_ID_GUID_data_3), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_3\n");
    return false;
  }
  if (fread(&(header.project_ID_GUID_data_4), sizeof(char), 8, file) != 8)
  {
    fprintf(stderr,"ERROR: reading header.project_ID_GUID_data_4\n");
    return false;
  }
  if (fread(&(header.version_major), sizeof(char), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.version_major\n");
    return false;
  }
  if (fread(&(header.version_minor), sizeof(char), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.version_minor\n");
    return false;
  }
  if (fread(&(header.system_identifier), sizeof(char), 32, file) != 32)
  {
    fprintf(stderr,"ERROR: reading header.system_identifier\n");
    return false;
  }
  if (fread(&(header.generating_software), sizeof(char), 32, file) != 32)
  {
    fprintf(stderr,"ERROR: reading header.generating_software\n");
    return false;
  }
  if (fread(&(header.file_creation_day), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.file_creation_day\n");
    return false;
  }
  if (fread(&(header.file_creation_year), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.file_creation_year\n");
    return false;
  }
  if (fread(&(header.header_size), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.header_size\n");
    return false;
  }
  if (fread(&(header.offset_to_point_data), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.offset_to_point_data\n");
    return false;
  }
  if (fread(&(header.number_of_variable_length_records), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.number_of_variable_length_records\n");
    return false;
  }
  if (fread(&(header.point_data_format), sizeof(unsigned char), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.point_data_format\n");
    return false;
  }
  if (fread(&(header.point_data_record_length), sizeof(unsigned short), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.point_data_record_length\n");
    return false;
  }
  if (fread(&(header.number_of_point_records), sizeof(unsigned int), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.number_of_point_records\n");
    return false;
  }
  if (fread(&(header.number_of_points_by_return), sizeof(unsigned int), 5, file) != 5)
  {
    fprintf(stderr,"ERROR: reading header.number_of_points_by_return\n");
    return false;
  }
  if (fread(&(header.x_scale_factor), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.x_scale_factor\n");
    return false;
  }
  if (fread(&(header.y_scale_factor), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.y_scale_factor\n");
    return false;
  }
  if (fread(&(header.z_scale_factor), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.z_scale_factor\n");
    return false;
  }
  if (fread(&(header.x_offset), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.x_offset\n");
    return false;
  }
  if (fread(&(header.y_offset), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.y_offset\n");
    return false;
  }
  if (fread(&(header.z_offset), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.z_offset\n");
    return false;
  }
  if (fread(&(header.max_x), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.max_x\n");
    return false;
  }
  if (fread(&(header.min_x), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.min_x\n");
    return false;
  }
  if (fread(&(header.max_y), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.max_y\n");
    return false;
  }
  if (fread(&(header.min_y), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.min_y\n");
    return false;
  }
  if (fread(&(header.max_z), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.max_z\n");
    return false;
  }
  if (fread(&(header.min_z), sizeof(double), 1, file) != 1)
  {
    fprintf(stderr,"ERROR: reading header.min_z\n");
    return false;
  }

  // check header contents

  if (strncmp(header.file_signature, "LASF", 4) != 0)
  {
    fprintf(stderr,"ERROR: wrong file signature '%s'\n", header.file_signature);
    return false;
  }
  if ((header.version_major != 1) || ((header.version_minor != 0) && (header.version_minor != 1) && (header.version_minor != 2)))
  {
    fprintf(stderr,"WARNING: unknown version %d.%d (should be 1.0 or 1.1 or 1.2)\n", header.version_major, header.version_minor);
  }
  if (header.header_size < 227)
  {
    fprintf(stderr,"WARNING: header size is %d but should be at least 227\n", header.header_size);
  }
  if (header.offset_to_point_data < header.header_size)
  {
    fprintf(stderr,"ERROR: offset to point data %d is smaller than header size %d\n", header.offset_to_point_data, header.header_size);
    return false;
  }
  if (header.number_of_point_records <= 0)
  {
    fprintf(stderr,"WARNING: number of point records is %d\n", header.number_of_point_records);
  }
  if ((header.point_data_format & 127) == 0)
  {
    if (header.point_data_record_length != 20)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 20 for format 0\n", header.point_data_record_length);
      if (header.point_data_record_length > 20)
      {
        additional_bytes_per_point = header.point_data_record_length - 20;
        fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
      }
      header.point_data_record_length = 20;
    }
  }
  else if ((header.point_data_format & 127) == 1)
  {
    if (header.point_data_record_length != 28)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 28 for format 1\n", header.point_data_record_length);
      if (header.point_data_record_length > 28)
      {
        additional_bytes_per_point = header.point_data_record_length - 28;
        fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
      }
      header.point_data_record_length = 28;
    }
  }
  else if ((header.point_data_format & 127) == 2)
  {
    if (header.point_data_record_length != 26)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 26 for format 2\n", header.point_data_record_length);
      if (header.point_data_record_length > 26)
      {
        additional_bytes_per_point = header.point_data_record_length - 26;
        fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
      }
      header.point_data_record_length = 26;
    }
  }
  else if ((header.point_data_format & 127) == 3)
  {
    if (header.point_data_record_length != 34)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 34 for format 3\n", header.point_data_record_length);
      if (header.point_data_record_length > 34)
      {
        additional_bytes_per_point = header.point_data_record_length - 34;
        fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
      }
      header.point_data_record_length = 34;
    }
  }
  else
  {
    fprintf(stderr,"WARNING: unknown point data format %d ... assuming format 0\n", header.point_data_format);
    if (header.point_data_record_length != 20)
    {
      fprintf(stderr,"WARNING: wrong point data record length of %d instead of 20 for format 0\n", header.point_data_record_length);
      if (header.point_data_record_length > 20)
      {
        additional_bytes_per_point = header.point_data_record_length - 20;
        fprintf(stderr,"WARNING: skipping the %d additional bytes per point\n", additional_bytes_per_point);
      }
      header.point_data_record_length = 20;
    }
    header.point_data_format = 0;
  }
  if (header.x_scale_factor == 0 || header.y_scale_factor == 0 || header.z_scale_factor == 0)
  {
    fprintf(stderr,"WARNING: some scale factors are zero %g %g %g. will set them to 0.01.\n", header.x_scale_factor, header.y_scale_factor, header.z_scale_factor);
    if (header.x_scale_factor == 0) header.x_scale_factor = 0.01;
    if (header.y_scale_factor == 0) header.y_scale_factor = 0.01;
    if (header.z_scale_factor == 0) header.z_scale_factor = 0.01;
  }
  if (header.max_x < header.min_x || header.max_y < header.min_y || header.max_z < header.min_z)
  {
    fprintf(stderr,"WARNING: invalid bounding box [ %g %g %g / %g %g %g ]\n", header.min_x, header.min_y, header.min_z, header.max_x, header.max_y, header.max_z);
  }

  // load any number of user-defined bytes that might have been added to the header
  header.user_data_in_header_size = header.header_size - 227;
  if (header.user_data_in_header_size)
  {
    header.user_data_in_header = new char[header.user_data_in_header_size];

    if (fread(header.user_data_in_header, sizeof(char), header.user_data_in_header_size, file) != (unsigned int)header.user_data_in_header_size)
    {
      fprintf(stderr,"ERROR: reading %d bytes of data into header.user_data_in_header\n", header.user_data_in_header_size);
      return false;
    }
  }

  // create the right point reader in dependance on compression and point data format

  if (header.point_data_format & 128)
  {
#ifdef ENABLE_LAS_COMPRESSION_SUPPORT
    // change the format to uncompressed
    header.point_data_format &= 127;
    switch (header.point_data_format)
    {
    case 0:
      pointReader = new LASpointReader0compressed(file);
      points_have_gps_time = false;
      points_have_rgb = false;
      break;
    case 1:
      pointReader = new LASpointReader1compressed(file);
      points_have_gps_time = true;
      points_have_rgb = false;
      break;
    case 2:
      pointReader = new LASpointReader2compressed(file);
      points_have_gps_time = false;
      points_have_rgb = true;
      break;
    case 3:
      pointReader = new LASpointReader3compressed(file);
      points_have_gps_time = true;
      points_have_rgb = true;
      break;
    }
#else // ENABLE_LAS_COMPRESSION_SUPPORT
    fprintf(stderr,"ERROR: this version of the lasreader does not support compression\n");
    return false;
#endif // ENABLE_LAS_COMPRESSION_SUPPORT
  }
  else
  {
    switch (header.point_data_format)
    {
    case 0:
      pointReader = new LASpointReader0raw(file);
      points_have_gps_time = false;
      points_have_rgb = false;
      break;
    case 1:
      pointReader = new LASpointReader1raw(file);
      points_have_gps_time = true;
      points_have_rgb = false;
      break;
    case 2:
      pointReader = new LASpointReader2raw(file);
      points_have_gps_time = false;
      points_have_rgb = true;
      break;
    case 3:
      pointReader = new LASpointReader3raw(file);
      points_have_gps_time = true;
      points_have_rgb = true;
      break;
    }
  }

  if (skip_all_headers)
  {

    for (i = header.header_size; i < (int)header.offset_to_point_data; i++) fgetc(file);
  }
  else
  {
    int vlrs_size = 0;

    // read the variable length records into the header
  
    if (header.number_of_variable_length_records)
    {
      header.vlrs = new LASvlr[header.number_of_variable_length_records];
    
      for (i = 0; i < (int)header.number_of_variable_length_records; i++)
      {
        // make sure there are enough bytes left to read a variable length record before the point block starts
        if (((int)header.offset_to_point_data - vlrs_size - header.header_size) < 54)
        {
          fprintf(stderr,"WARNING: only %d bytes until point block after reading %d of %d vlrs. skipping remaining vlrs ...\n", (int)header.offset_to_point_data - vlrs_size - header.header_size, i, header.number_of_variable_length_records);
          header.number_of_variable_length_records = i;
          break;
        }

        // read variable length records variable after variable (to avoid alignment issues)

        if (fread(&(header.vlrs[i].reserved), sizeof(unsigned short), 1, file) != 1)
        {
          fprintf(stderr,"ERROR: reading header.vlrs[%d].reserved\n", i);
          return false;
        }
        if (fread(header.vlrs[i].user_id, sizeof(char), 16, file) != 16)
        {
          fprintf(stderr,"ERROR: reading header.vlrs[%d].user_id\n", i);
          return false;
        }
        if (fread(&(header.vlrs[i].record_id), sizeof(unsigned short), 1, file) != 1)
        {
          fprintf(stderr,"ERROR: reading header.vlrs[%d].record_id\n", i);
          return false;
        }
        if (fread(&(header.vlrs[i].record_length_after_header), sizeof(unsigned short), 1, file) != 1)
        {
          fprintf(stderr,"ERROR: reading header.vlrs[%d].record_length_after_header\n", i);
          return false;
        }
        if (fread(header.vlrs[i].description, sizeof(char), 32, file) != 32)
        {
          fprintf(stderr,"ERROR: reading header.vlrs[%d].description\n", i);
          return false;
        }

        // keep track on the number of bytes we have read so far

        vlrs_size += 54;

        // check variable length record contents

        if (header.vlrs[i].reserved != 0xAABB)
        {
          fprintf(stderr,"WARNING: wrong header.vlrs[%d].reserved: %d != 0xAABB\n", i, header.vlrs[i].reserved);
        }

        // make sure there are enough bytes left to read the data of the variable length record before the point block starts

        if (((int)header.offset_to_point_data - vlrs_size - header.header_size) < header.vlrs[i].record_length_after_header)
        {
          fprintf(stderr,"WARNING: only %d bytes until point block when trying to read %d bytes into header.vlrs[%d].data\n", (int)header.offset_to_point_data - vlrs_size - header.header_size, header.vlrs[i].record_length_after_header, i);
          header.vlrs[i].record_length_after_header = (int)header.offset_to_point_data - vlrs_size - header.header_size;
        }

        // load data following the header of the variable length record

        if (header.vlrs[i].record_length_after_header)
        {
          header.vlrs[i].data = new char[header.vlrs[i].record_length_after_header];

          if (fread(header.vlrs[i].data, sizeof(char), header.vlrs[i].record_length_after_header, file) != header.vlrs[i].record_length_after_header)
          {
            fprintf(stderr,"ERROR: reading %d bytes of data into header.vlrs[%d].data\n", header.vlrs[i].record_length_after_header, i);
            return false;
          }
        }
        else
        {
          header.vlrs[i].data = 0;
        }

        // keep track on the number of bytes we have read so far

        vlrs_size += header.vlrs[i].record_length_after_header;

        // special handling for known variable header tags

        if (strcmp(header.vlrs[i].user_id, "LASF_Projection") == 0)
        {
          if (header.vlrs[i].record_id == 34735) // GeoKeyDirectoryTag
          {
            if (header.vlr_geo_keys)
            {
              fprintf(stderr,"WARNING: variable length records contain more than one GeoKeyDirectoryTag\n");
            }
            header.vlr_geo_keys = (LASvlr_geo_keys*)header.vlrs[i].data;

            // check variable header geo keys contents

            if (header.vlr_geo_keys->key_directory_version != 1)
            {
              fprintf(stderr,"WARNING: wrong vlr_geo_keys->key_directory_version: %d != 1\n",header.vlr_geo_keys->key_directory_version);
            }
            if (header.vlr_geo_keys->key_revision != 1)
            {
              fprintf(stderr,"WARNING: wrong vlr_geo_keys->key_revision: %d != 1\n",header.vlr_geo_keys->key_revision);
            }
            if (header.vlr_geo_keys->minor_revision != 0)
            {
              fprintf(stderr,"WARNING: wrong vlr_geo_keys->minor_revision: %d != 0\n",header.vlr_geo_keys->minor_revision);
            }
            header.vlr_geo_key_entries = (LASvlr_key_entry*)&header.vlr_geo_keys[1];
          }
          else if (header.vlrs[i].record_id == 34736) // GeoDoubleParamsTag
          {
            if (header.vlr_geo_double_params)
            {
              fprintf(stderr,"WARNING: variable length records contain more than one GeoDoubleParamsTag\n");
            }
            header.vlr_geo_double_params = (double*)header.vlrs[i].data;
          }
          else if (header.vlrs[i].record_id == 34737) // GeoAsciiParamsTag
          {
            if (header.vlr_geo_ascii_params)
            {
              fprintf(stderr,"WARNING: variable length records contain more than one GeoAsciiParamsTag\n");
            }
            header.vlr_geo_ascii_params = (char*)header.vlrs[i].data;
          }
        }
      }
    }

    // load any number of user-defined bytes that might have been added after the header

    header.user_data_after_header_size = (int)header.offset_to_point_data - vlrs_size - header.header_size;
    if (header.user_data_after_header_size)
    {
      header.user_data_after_header = new char[header.user_data_after_header_size];

      if (fread(header.user_data_after_header, sizeof(char), header.user_data_after_header_size, file) != (unsigned int)header.user_data_after_header_size)
      {
        fprintf(stderr,"ERROR: reading %d bytes of data into header.user_data_after_header\n", header.user_data_after_header_size);
        return false;
      }
    }
  }

  gps_time = 0.0;
  rgb[0] = rgb[1] = rgb[2] = 0;
  npoints = header.number_of_point_records;
  p_count = 0;

  return true;
}

bool LASreader::read_point()
{
  if (p_count < npoints)
  {
    if (pointReader->read_point(&point, &gps_time, rgb) == false)
    {
      fprintf(stderr,"WARNING: end-of-file after %d of %d points\n", p_count, npoints);
      return false;
    }
    if (additional_bytes_per_point)
    {
      for (int i = 0; i < additional_bytes_per_point; i++) fgetc(file);
    }
    p_count++;
    return true;
  }
  return false;
}

bool LASreader::read_point(float* coordinates)
{
  if (read_point())
  {
    get_coordinates(coordinates);
    return true;
  }
  return false;
}

bool LASreader::read_point(double* coordinates)
{
  if (read_point())
  {
    get_coordinates(coordinates);
    return true;
  }
  return false;
}

void LASreader::get_coordinates(float* coordinates)
{
  coordinates[0] = (float)(point.x*header.x_scale_factor+header.x_offset);
  coordinates[1] = (float)(point.y*header.y_scale_factor+header.y_offset);
  coordinates[2] = (float)(point.z*header.z_scale_factor+header.z_offset);
}

void LASreader::get_coordinates(double* coordinates)
{
  coordinates[0] = point.x*header.x_scale_factor+header.x_offset;
  coordinates[1] = point.y*header.y_scale_factor+header.y_offset;
  coordinates[2] = point.z*header.z_scale_factor+header.z_offset;
}

void LASreader::close()
{
  file = 0;

  if (pointReader) 
  {
    delete pointReader;
    pointReader = 0;
  }
}

LASreader::LASreader()
{
  gps_time = 0.0;
  rgb[0] = rgb[1] = rgb[2] = 0;
  points_have_gps_time = false;
  points_have_rgb = false;
  npoints = -1;
  p_count = -1;
  additional_bytes_per_point = 0;
  file = 0;
  pointReader = 0;
}

LASreader::~LASreader()
{
}
