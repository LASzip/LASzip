/*
===============================================================================

  FILE:  lasdiff.cpp

  CONTENTS:

    This tool reads two LIDAR files in LAS format and checks whether they are
    identical. Both the standard header, the variable header, and all points
    are checked.

  PROGRAMMERS:

    martin isenburg@cs.unc.edu

  COPYRIGHT:

    copyright (C) 2007-08  martin isenburg@cs.unc.edu

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  CHANGE HISTORY:

    12 March 2009 -- updated to ask for input if started without arguments 
    17 September 2008 -- updated to deal with LAS format version 1.2
    11 July 2007 -- added more complete reporting about differences
    23 February 2007 -- created just before getting ready for the cabin trip

===============================================================================
*/

#include "lasreader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void usage(bool wait=false)
{
  fprintf(stderr,"usage:\n");
  fprintf(stderr,"lasdiff lidar1.las lidar2.las\n");
  fprintf(stderr,"lasdiff lidar1.las lidar1.las.lz\n");
  fprintf(stderr,"lasdiff -i lidar1.las -i lidar1.laz\n");
  fprintf(stderr,"lasdiff -h\n");
  if (wait)
  {
    fprintf(stderr,"<press ENTER>\n");
    getc(stdin);
  }
  exit(1);
}

static void byebye(bool wait=false)
{
  if (wait)
  {
    fprintf(stderr,"<press ENTER>\n");
    getc(stdin);
  }
  exit(1);
}

static int lidardouble2string(char* string, double value0, double value1)
{
  int len;
  len = sprintf(string, "%f", value0) - 1;
  while (string[len] == '0') len--;
  if (string[len] != '.') len++;
  len += sprintf(&(string[len]), " %f", value1) - 1;
  while (string[len] == '0') len--;
  if (string[len] != '.') len++;
  string[len] = '\0';
  return len;
}

int main(int argc, char *argv[])
{
  int i,j;
  char* file_name_in1 = 0;
  char* file_name_in2 = 0;

  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i],"-i") == 0)
    {
      i++;
      if (file_name_in1) file_name_in2 = argv[i];
      else file_name_in1 = argv[i];
    }
    else if (i == argc - 2 && file_name_in1 == 0 && file_name_in2 == 0)
    {
      if (file_name_in1) file_name_in2 = argv[i];
      else file_name_in1 = argv[i];
    }
    else if (i == argc - 1 && file_name_in1 && file_name_in2 == 0)
    {
      if (file_name_in1) file_name_in2 = argv[i];
      else file_name_in1 = argv[i];
    }
    else
    {
      fprintf(stderr,"cannot understand argument '%s'\n", argv[i]);
      usage();
    }
  }

  if (argc == 1)
  {
    fprintf(stderr,"lasdiff.exe is better run in the command line\n");
    file_name_in1 = new char[256];
    fprintf(stderr,"enter input file1: "); fgets(file_name_in1, 256, stdin);
    file_name_in1[strlen(file_name_in1)-1] = '\0';
    file_name_in2 = new char[256];
    fprintf(stderr,"enter input file2: "); fgets(file_name_in2, 256, stdin);
    file_name_in2[strlen(file_name_in2)-1] = '\0';
  }

  FILE* file_in1 = 0;
  if (file_name_in1)
  {
    file_in1 = fopen(file_name_in1, "rb");
    if (file_in1 == 0)
    {
      fprintf (stderr, "ERROR: could not open file '%s'\n", file_name_in1);
      usage(argc==1);
    }
  }
  else
  {
    fprintf (stderr, "ERROR: no input specified\n");
    usage(argc==1);
  }

  LASreader* lasreader1 = new LASreader();
  if (!lasreader1->open(file_in1))
  {
    fprintf (stderr, "ERROR: lasreader open failed for '%s'\n", file_name_in1);
    usage(argc==1);
  }

  FILE* file_in2 = 0;
  if (file_name_in2)
  {
    file_in2 = fopen(file_name_in2, "rb");
    if (file_in2 == 0)
    {
      fprintf (stderr, "ERROR: could not open file '%s'\n", file_name_in2);
      usage(argc==1);
    }
  }
  else
  {
    fprintf (stderr, "ERROR: no input specified\n");
    usage(argc==1);
  }

  LASreader* lasreader2 = new LASreader();
  if (!lasreader2->open(file_in2))
  {
    fprintf (stderr, "ERROR: lasreader open failed for '%s'\n", file_name_in2);
    usage(argc==1);
  }

  // check header

  int different_header = 0;

  int memcmp_until = (int)(((const char*)&(lasreader1->header.user_data_in_header_size))-((const char*)&(lasreader1->header)));

  if (memcmp((const void*)&(lasreader1->header), (const void*)&(lasreader2->header), memcmp_until))
  {
    char printstring[128];
    
    fprintf(stderr, "header is different\n");

    LASheader* lasheader1 = &(lasreader1->header);
    LASheader* lasheader2 = &(lasreader2->header);

    bool fatal_difference = false;

    if (strncmp(lasheader1->file_signature, lasheader2->file_signature, 4))
    {
      fprintf(stderr, "  different file_signature: %s %s\n", lasheader1->file_signature, lasheader2->file_signature);
      different_header++;
    }
    if (lasheader2->file_source_id != lasheader2->file_source_id)
    {
      fprintf(stderr, "  different file_source_id: %d %d\n", lasheader1->file_source_id, lasheader2->file_source_id);
      different_header++;
    }
    if (lasheader1->global_encoding != lasheader2->global_encoding)
    {
      fprintf(stderr, "  different reserved (global_encoding): %d %d\n", lasheader1->global_encoding, lasheader2->global_encoding);
      different_header++;
    }
    if (lasheader1->project_ID_GUID_data_1 != lasheader2->project_ID_GUID_data_1)
    {
      fprintf(stderr, "  different project_ID_GUID_data_1: %d %d\n", lasheader1->project_ID_GUID_data_1, lasheader2->project_ID_GUID_data_1);
      different_header++;
    }
    if (lasheader1->project_ID_GUID_data_2 != lasheader2->project_ID_GUID_data_2)
    {
      fprintf(stderr, "  different project_ID_GUID_data_2: %d %d\n", lasheader1->project_ID_GUID_data_2, lasheader2->project_ID_GUID_data_2);
      different_header++;
    }
    if (lasheader1->project_ID_GUID_data_3 != lasheader2->project_ID_GUID_data_3)
    {
      fprintf(stderr, "  different project_ID_GUID_data_3: %d %d\n", lasheader1->project_ID_GUID_data_3, lasheader2->project_ID_GUID_data_3);
      different_header++;
    }
    if (strncmp(lasheader1->project_ID_GUID_data_4, lasheader2->project_ID_GUID_data_4, 8))
    {
      fprintf(stderr, "  different project_ID_GUID_data_4: %s %s\n", lasheader1->project_ID_GUID_data_4, lasheader2->project_ID_GUID_data_4);
      different_header++;
    }
    if (lasheader1->version_major != lasheader2->version_major || lasheader1->version_minor != lasheader2->version_minor)
    {
      fprintf(stderr, "  different version: %d.%d %d.%d\n", lasheader1->version_major, lasheader1->version_minor, lasheader2->version_major, lasheader2->version_minor);
      different_header++;
    }
    if (strncmp(lasheader1->system_identifier, lasheader2->system_identifier, 32))
    {
      fprintf(stderr, "  different system_identifier: %s %s\n", lasheader1->system_identifier, lasheader2->system_identifier);
      different_header++;
    }
    if (strncmp(lasheader1->generating_software, lasheader2->generating_software, 32))
    {
      fprintf(stderr, "  different generating_software: %s %s\n", lasheader1->generating_software, lasheader2->generating_software);
      different_header++;
    }
    if (lasheader1->file_creation_day != lasheader2->file_creation_day || lasheader1->file_creation_year != lasheader2->file_creation_year)
    {
      fprintf(stderr, "  different file_creation day.year: %d.%d %d.%d\n", lasheader1->file_creation_day, lasheader1->file_creation_year, lasheader2->file_creation_day, lasheader2->file_creation_year);
      different_header++;
    }
    if (lasheader1->header_size != lasheader2->header_size)
    {
      fprintf(stderr, "  different header_size: %d %d\n", lasheader1->header_size, lasheader2->header_size);
      fatal_difference = true;
    }
    if (lasheader1->offset_to_point_data != lasheader2->offset_to_point_data)
    {
      fprintf(stderr, "  different offset_to_point_data: %d %d\n", lasheader1->offset_to_point_data, lasheader2->offset_to_point_data);
      different_header++;
    }
    if (lasheader1->point_data_format != lasheader2->point_data_format)
    {
      fprintf(stderr, "  different point_data_format: %d %d\n", lasheader1->point_data_format, lasheader2->point_data_format);
      fatal_difference = true;
    }
    if (lasheader1->point_data_record_length != lasheader2->point_data_record_length)
    {
      fprintf(stderr, "  different point_data_record_length: %d %d\n", lasheader1->point_data_record_length, lasheader2->point_data_record_length);
      fatal_difference = true;
    }
    if (lasheader1->number_of_point_records != lasheader2->number_of_point_records)
    {
      fprintf(stderr, "  different number_of_point_records: %d %d\n", lasheader1->number_of_point_records, lasheader2->number_of_point_records);
      different_header++;
    }
    if (lasheader1->number_of_points_by_return[0] != lasheader2->number_of_points_by_return[0] || lasheader1->number_of_points_by_return[1] != lasheader2->number_of_points_by_return[1] || lasheader1->number_of_points_by_return[2] != lasheader2->number_of_points_by_return[2] || lasheader1->number_of_points_by_return[3] != lasheader2->number_of_points_by_return[3] || lasheader1->number_of_points_by_return[4] != lasheader2->number_of_points_by_return[4])
    {
      fprintf(stderr, "  different number_of_points_by_return: (%d,%d,%d,%d,%d) (%d,%d,%d,%d,%d)\n", lasheader1->number_of_points_by_return[0], lasheader1->number_of_points_by_return[1], lasheader1->number_of_points_by_return[2], lasheader1->number_of_points_by_return[3], lasheader1->number_of_points_by_return[4], lasheader2->number_of_points_by_return[0], lasheader2->number_of_points_by_return[1], lasheader2->number_of_points_by_return[2], lasheader2->number_of_points_by_return[3], lasheader2->number_of_points_by_return[4]);
      different_header++;
    }
    if (lasheader1->x_scale_factor != lasheader2->x_scale_factor)
    {
      lidardouble2string(printstring, lasheader1->x_scale_factor, lasheader2->x_scale_factor); fprintf(stderr, "  different x_scale_factor: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->y_scale_factor != lasheader2->y_scale_factor)
    {
      lidardouble2string(printstring, lasheader1->y_scale_factor, lasheader2->y_scale_factor); fprintf(stderr, "  different y_scale_factor: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->z_scale_factor != lasheader2->z_scale_factor)
    {
      lidardouble2string(printstring, lasheader1->z_scale_factor, lasheader2->z_scale_factor); fprintf(stderr, "  different z_scale_factor: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->x_offset != lasheader2->x_offset)
    {
      lidardouble2string(printstring, lasheader1->x_offset, lasheader2->x_offset); fprintf(stderr, "  different x_offset: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->y_offset != lasheader2->y_offset)
    {
      lidardouble2string(printstring, lasheader1->y_offset, lasheader2->y_offset); fprintf(stderr, "  different y_offset: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->z_offset != lasheader2->z_offset)
    {
      lidardouble2string(printstring, lasheader1->z_offset, lasheader2->z_offset); fprintf(stderr, "  different z_offset: %s\n", printstring);
      fatal_difference = true;
    }
    if (lasheader1->max_x != lasheader2->max_x)
    {
      lidardouble2string(printstring, lasheader1->max_x, lasheader2->max_x); fprintf(stderr, "  different max_x: %s\n", printstring);
      different_header++;
    }
    if (lasheader1->min_x != lasheader2->min_x)
    {
      lidardouble2string(printstring, lasheader1->min_x, lasheader2->min_x); fprintf(stderr, "  different min_x: %s\n", printstring);
      different_header++;
    }
    if (lasheader1->max_y != lasheader2->max_y)
    {
      lidardouble2string(printstring, lasheader1->max_y, lasheader2->max_y); fprintf(stderr, "  different max_y: %s\n", printstring);
      different_header++;
    }
    if (lasheader1->min_y != lasheader2->min_y)
    {
      lidardouble2string(printstring, lasheader1->min_y, lasheader2->min_y); fprintf(stderr, "  different min_y: %s\n", printstring);
      different_header++;
    }
    if (lasheader1->max_z != lasheader2->max_z)
    {
      lidardouble2string(printstring, lasheader1->max_z, lasheader2->max_z); fprintf(stderr, "  different max_z: %s\n", printstring);
      different_header++;
    }
    if (lasheader1->min_z != lasheader2->min_z)
    {
      lidardouble2string(printstring, lasheader1->min_z, lasheader2->min_z); fprintf(stderr, "  different min_z: %s\n", printstring);
      different_header++;
    }
    if (fatal_difference)
    {
      fprintf(stderr, "difference was fatal ... no need to check points\n");
      byebye(argc==1);
    }
  }

  // check user-defined data in header

  if (lasreader1->header.user_data_in_header_size == lasreader2->header.user_data_in_header_size)
  {
    for (i = 0; i < lasreader1->header.user_data_in_header_size; i++)
    {
      if (lasreader1->header.user_data_in_header[i] != lasreader2->header.user_data_in_header[i])
      {
        different_header++;
        fprintf(stderr, "user-defined data in header is different at byte %d of %d\n", i, lasreader1->header.user_data_in_header_size);
        break;
      }
    }
  }
  else
  {
    different_header++;
    fprintf(stderr, "skipping check of user-defined data in header due to length difference (%d != %d)\n", lasreader1->header.user_data_in_header_size, lasreader2->header.user_data_in_header_size);
  }

  // check variable header (and user data)

  if (lasreader1->header.number_of_variable_length_records == lasreader2->header.number_of_variable_length_records)
  {
    for (i = 0; i < (int)lasreader1->header.number_of_variable_length_records; i++)
    {
      if (lasreader1->header.vlrs[i].reserved != lasreader2->header.vlrs[i].reserved)
      {
        fprintf(stderr, "variable length record %d reserved field is different: %d versus %d\n", i, lasreader1->header.vlrs[i].reserved, lasreader2->header.vlrs[i].reserved);
      }
      if (memcmp(lasreader1->header.vlrs[i].user_id, lasreader2->header.vlrs[i].user_id, 16) != 0)
      {
        fprintf(stderr, "variable length record %d user_id field is different: %s versus %s\n", i, lasreader1->header.vlrs[i].user_id, lasreader2->header.vlrs[i].user_id);
      }
      if (lasreader1->header.vlrs[i].record_id != lasreader2->header.vlrs[i].record_id)
      {
        fprintf(stderr, "variable length record %d record_id field is different: %d versus %d\n", i, lasreader1->header.vlrs[i].record_id, lasreader2->header.vlrs[i].record_id);
      }
      if (lasreader1->header.vlrs[i].record_length_after_header != lasreader2->header.vlrs[i].record_length_after_header)
      {
        fprintf(stderr, "variable length record %d record_length_after_header field is different: %d versus %d\n", i, lasreader1->header.vlrs[i].record_length_after_header, lasreader2->header.vlrs[i].record_length_after_header);
      }
      if (memcmp(lasreader1->header.vlrs[i].description, lasreader2->header.vlrs[i].description, 32) != 0)
      {
        fprintf(stderr, "variable length record %d description field is different: %s versus %s\n", i, lasreader1->header.vlrs[i].description, lasreader2->header.vlrs[i].description);
      }
      if (memcmp(lasreader1->header.vlrs[i].data, lasreader2->header.vlrs[i].data, lasreader1->header.vlrs[i].record_length_after_header))
      {
        for (j = 0; j < lasreader1->header.vlrs[i].record_length_after_header; j++)
        {
          if (lasreader1->header.vlrs[i].data[j] != lasreader2->header.vlrs[i].data[j])
          {
            different_header++;
            fprintf(stderr, "variable length record %d data field is different at byte %d: %d versus %d\n", i, j, lasreader1->header.vlrs[i].data[j], lasreader2->header.vlrs[i].data[j]);
            break;
          }
        }
      }
    }
  }
  else
  {
    different_header++;
    fprintf(stderr, "skipping check of variable length records (or user data) due to different number (%d != %d)\n", lasreader1->header.number_of_variable_length_records, lasreader2->header.number_of_variable_length_records);
  }

  // check user-defined data after header

  if (lasreader1->header.user_data_after_header_size == lasreader2->header.user_data_after_header_size)
  {
    for (i = 0; i < lasreader1->header.user_data_after_header_size; i++)
    {
      if (lasreader1->header.user_data_after_header[i] != lasreader2->header.user_data_after_header[i])
      {
        different_header++;
        fprintf(stderr, "user-defined data after header is different at byte %d of %d\n", i, lasreader1->header.user_data_in_header_size);
        break;
      }
    }
  }
  else
  {
    different_header++;
    fprintf(stderr, "skipping check of user-defined data in header due to length difference (%d != %d)\n", lasreader1->header.user_data_after_header_size, lasreader2->header.user_data_after_header_size);
  }

  // check points

  int different_points = 0;

  while (true)
  {
    bool difference = false;
    if (lasreader1->read_point())
    {
      if (lasreader2->read_point())
      {
        if (memcmp((const void*)&(lasreader1->point), (const void*)&(lasreader2->point), sizeof(LASpoint)))
        {
          fprintf(stderr, "point %d of %d is different\n", lasreader1->p_count, lasreader1->npoints);
          if (lasreader1->point.x != lasreader2->point.x)
          {
            fprintf(stderr, "  x: %d %d\n", lasreader1->point.x, lasreader2->point.x);
          }
          if (lasreader1->point.y != lasreader2->point.y)
          {
            fprintf(stderr, "  y: %d %d\n", lasreader1->point.y, lasreader2->point.y);
          }
          if (lasreader1->point.z != lasreader2->point.z)
          {
            fprintf(stderr, "  z: %d %d\n", lasreader1->point.z, lasreader2->point.z);
          }
          if (lasreader1->point.intensity != lasreader2->point.intensity)
          {
            fprintf(stderr, "  intensity: %d %d\n", lasreader1->point.intensity, lasreader2->point.intensity);
          }
          if (lasreader1->point.return_number != lasreader2->point.return_number)
          {
            fprintf(stderr, "  return_number: %d %d\n", lasreader1->point.return_number, lasreader2->point.return_number);
          }
          if (lasreader1->point.number_of_returns_of_given_pulse != lasreader2->point.number_of_returns_of_given_pulse)
          {
            fprintf(stderr, "  number_of_returns_of_given_pulse: %d %d\n", lasreader1->point.number_of_returns_of_given_pulse, lasreader2->point.number_of_returns_of_given_pulse);
          }
          if (lasreader1->point.scan_direction_flag != lasreader2->point.scan_direction_flag)
          {
            fprintf(stderr, "  scan_direction_flag: %d %d\n", lasreader1->point.scan_direction_flag, lasreader2->point.scan_direction_flag);
          }
          if (lasreader1->point.edge_of_flight_line != lasreader2->point.edge_of_flight_line)
          {
            fprintf(stderr, "  edge_of_flight_line: %d %d\n", lasreader1->point.edge_of_flight_line, lasreader2->point.edge_of_flight_line);
          }
          if (lasreader1->point.classification != lasreader2->point.classification)
          {
            fprintf(stderr, "  classification: %d %d\n", lasreader1->point.classification, lasreader2->point.classification);
          }
          if (lasreader1->point.scan_angle_rank != lasreader2->point.scan_angle_rank)
          {
            fprintf(stderr, "  scan_angle_rank: %d %d\n", lasreader1->point.scan_angle_rank, lasreader2->point.scan_angle_rank);
          }
          if (lasreader1->point.user_data != lasreader2->point.user_data)
          {
            fprintf(stderr, "  user_data: %d %d\n", lasreader1->point.user_data, lasreader2->point.user_data);
          }
          if (lasreader1->point.point_source_ID != lasreader2->point.point_source_ID)
          {
            fprintf(stderr, "  point_source_ID: %d %d\n", lasreader1->point.point_source_ID, lasreader2->point.point_source_ID);
          }
          difference = true;
        }
        if (lasreader1->points_have_gps_time)
        {
          if (lasreader1->gps_time != lasreader2->gps_time)
          {
            fprintf(stderr, "gps time of point %d of %d is different: %f != %f\n", lasreader1->p_count, lasreader1->npoints, lasreader1->gps_time, lasreader2->gps_time);
            difference = true;
          }
        }
        if (lasreader1->points_have_rgb)
        {
          if (memcmp((const void*)&(lasreader1->rgb), (const void*)&(lasreader2->rgb), sizeof(short[3])))
          {
            fprintf(stderr, "rgb of point %d of %d is different: (%d %d %d) != (%d %d %d)\n", lasreader1->p_count, lasreader1->npoints, lasreader1->rgb[0], lasreader1->rgb[1], lasreader1->rgb[2], lasreader2->rgb[0], lasreader2->rgb[1], lasreader2->rgb[2]);
            difference = true;
          }
        }
      }
      else
      {
        fprintf(stderr, "%s (%d) has fewer points than %s (%d)\n", file_name_in2, lasreader2->p_count, file_name_in1, lasreader1->p_count);
        byebye(argc==1);
      }
    }
    else
    {
      if (lasreader2->read_point())
      {
        fprintf(stderr, "%s (%d) has more points than %s (%d)\n", file_name_in2, lasreader2->p_count, file_name_in1, lasreader1->p_count);
        byebye(argc==1);
      }
      else
      {
        break;
      }
    }
    if (difference)
    {
      different_points++;
      if (different_points >= 5)
      {
        byebye(argc==1);
      }
    }
  }

  if (different_points) fprintf(stderr, "points are different. ");
  else fprintf(stderr, "points are identical. ");
  if (!different_header && !different_points) fprintf(stderr, "files are identical. ");
    
  fprintf(stderr, "both have %d points.\n", lasreader1->p_count);

  lasreader1->close();
  lasreader2->close();

  fclose(file_in1);
  fclose(file_in2);

  byebye(argc==1);

  return 0;
}
