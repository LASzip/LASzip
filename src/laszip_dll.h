/*
===============================================================================

  FILE:  laszip_dll.h

  CONTENTS:

    A simple DLL interface to LASzip

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2007-2017, martin isenburg, rapidlasso - fast tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the COPYING file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  CHANGE HISTORY:
   11 January 2018 -- moved declarations to header file
   30 August 2017 -- completing stream-based writing (with writing LAS header)
   23 August 2017 -- turn on "native" by default
    3 August 2017 -- new 'laszip_create_laszip_vlr()' gets VLR as C++ std::vector
   29 July 2017 -- integrating minimal stream-based reading/writing into branch
   20 July 2017 -- Andrew Bell adds support for stream-based reading/writing
   28 May 2017 -- support for "LAS 1.4 selective decompression" added into DLL API
   25 April 2017 -- adding initial support for new "native LAS 1.4 extension"
    8 January 2017 -- changed from "laszip_dll.h" to "laszip_api.h" for hobu

===============================================================================
*/

#include <laszip/laszip_api.h>

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_get_version(
    laszip_U8*                         version_major
    , laszip_U8*                       version_minor
    , laszip_U16*                      version_revision
    , laszip_U32*                      version_build
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_get_error(
    laszip_POINTER                     pointer
    , laszip_CHAR**                    error
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_get_warning(
    laszip_POINTER                     pointer
    , laszip_CHAR**                    warning
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_create(
    laszip_POINTER*                    pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_clean(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_destroy(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_get_header_pointer(
    laszip_POINTER                     pointer
    , laszip_header_struct**           header_pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_get_point_pointer(
    laszip_POINTER                     pointer
    , laszip_point_struct**            point_pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_get_point_count(
    laszip_POINTER                     pointer
    , laszip_I64*                      count
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_header(
    laszip_POINTER                     pointer
    , const laszip_header_struct*      header
);


/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_point_type_and_size(
    laszip_POINTER                     pointer
    , laszip_U8                        point_type
    , laszip_U16                       point_size
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_check_for_integer_overflow(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_auto_offset(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_point(
    laszip_POINTER                     pointer
    , const laszip_point_struct*       point
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_coordinates(
    laszip_POINTER                     pointer
    , const laszip_F64*                coordinates
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_get_coordinates(
    laszip_POINTER                     pointer
    , laszip_F64*                      coordinates
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_geokeys(
    laszip_POINTER                     pointer
    , laszip_U32                       number
    , const laszip_geokey_struct*      key_entries
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_geodouble_params(
    laszip_POINTER                     pointer
    , laszip_U32                       number
    , const laszip_F64*                geodouble_params
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_geoascii_params(
    laszip_POINTER                     pointer
    , laszip_U32                       number
    , const laszip_CHAR*               geoascii_params
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_add_attribute(
    laszip_POINTER                     pointer
    , laszip_U32                       type
    , const laszip_CHAR*               name
    , const laszip_CHAR*               description
    , laszip_F64                       scale
    , laszip_F64                       offset
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_add_vlr(
    laszip_POINTER                     pointer
    , const laszip_CHAR*               user_id
    , laszip_U16                       record_id
    , laszip_U16                       record_length_after_header
    , const laszip_CHAR*               description
    , const laszip_U8*                 data
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_remove_vlr(
    laszip_POINTER                     pointer
    , const laszip_CHAR*               user_id
    , laszip_U16                       record_id
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_preserve_generating_software(
    laszip_POINTER                     pointer
    , const laszip_BOOL                preserve
);


/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_request_native_extension(
    laszip_POINTER                     pointer
    , const laszip_BOOL                request
)
;

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_request_compatibility_mode(
    laszip_POINTER                     pointer
    , const laszip_BOOL                request
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_set_chunk_size(
    laszip_POINTER                     pointer
    , const laszip_U32                 chunk_size
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_create_spatial_index(
    laszip_POINTER                     pointer
    , const laszip_BOOL                create
    , const laszip_BOOL                append
);


/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_open_writer(
    laszip_POINTER                     pointer
    , const laszip_CHAR*               file_name
    , laszip_BOOL                      compress
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_write_point(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_write_indexed_point(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_update_inventory(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_close_writer(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_exploit_spatial_index(
    laszip_POINTER                     pointer
    , const laszip_BOOL                exploit
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_decompress_selective(
    laszip_POINTER                     pointer
    , const laszip_U32                 decompress_selective
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_open_reader(
    laszip_POINTER                     pointer
    , const laszip_CHAR*               file_name
    , laszip_BOOL*                     is_compressed
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_has_spatial_index(
    laszip_POINTER                     pointer
    , laszip_BOOL*                     is_indexed
    , laszip_BOOL*                     is_appended
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_inside_rectangle(
    laszip_POINTER                     pointer
    , const laszip_F64                 r_min_x
    , const laszip_F64                 r_min_y
    , const laszip_F64                 r_max_x
    , const laszip_F64                 r_max_y
    , laszip_BOOL*                     is_empty
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_seek_point(
    laszip_POINTER                     pointer
    , laszip_I64                       index
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_read_point(
    laszip_POINTER                     pointer
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_read_inside_point(
    laszip_POINTER                     pointer
    , laszip_BOOL*                     is_done
);

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_close_reader(
    laszip_POINTER                     pointer
);

#ifdef __cplusplus

/*---------------------------------------------------------------------------*/
LASZIP_API laszip_I32
laszip_open_reader_stream(
    laszip_POINTER                     pointer
    , istream&                         stream
    , laszip_BOOL*                     is_compressed
);

/*---------------------------------------------------------------------------*/
// The stream writer also supports software that writes the LAS header on its
// own simply by setting the BOOL 'do_not_write_header' to TRUE. This function
// should then be called just prior to writing points as data is then written
// to the current stream position
LASZIP_API laszip_I32
laszip_open_writer_stream(
    laszip_POINTER                     pointer
    , ostream&                         stream
    , laszip_BOOL                      compress
    , laszip_BOOL                      do_not_write_header
);

/*---------------------------------------------------------------------------*/
// creates complete LASzip VLR for currently selected point type and compression
// The VLR data is valid until the laszip_dll pointer is destroyed.
LASZIP_API laszip_I32
laszip_create_laszip_vlr(
    laszip_POINTER                     pointer
    , laszip_U8**                      vlr
    , laszip_U32*                      vlr_size
);

#endif // __cplusplus
