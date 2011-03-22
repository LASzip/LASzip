.. _home:

******************************************************************************
LASzip - lossless LAS compression
******************************************************************************


Download
------------------------------------------------------------------------------

Please note that `libLAS`_ links to the LASzip compression library while
`LASlib`_ embeds the LASzip codebase.

Source
..............................................................................

* **2011-02-01** 

  - `laszip-1.0.1-src.tar.gz <http://download.osgeo.org/laszip/laszip-1.0.1.tar.gz>`_ 
    `(md5) <http://download.osgeo.org/laszip/laszip-1.0.1.tar.gz.md5>`__ 

  - `laszip-1.0.1-src.zip <http://download.osgeo.org/laszip/laszip-1.0.0.zip>`_ 
    `(md5) <http://download.osgeo.org/liblas/laszip-1.0.1.zip.md5>`__

Binaries
..............................................................................

  - See `OSGeo4W`_ for Windows .lib and 
    include files to compile LASzip into your own application. `libLAS`_ 
    binaries through `OSGeo4W`_ also link LASzip.

  - A Windows binary `laszip.exe`_ and a simple makefile to build `laszip`_ are
    available for explicit compression (decompression) of LAS (LAZ) files.    


Development Source
------------------------------------------------------------------------------

* Current Trunk: http://hg.liblas.org/zip


Background
------------------------------------------------------------------------------

LASzip is a compression library that was developed by `Martin Isenburg`_ for
compressing `ASPRS LAS format`_ data in his `LAStools`_. It has been provided
as an `LGPL`_-licensed stand-alone software library to allow other softwares
that handle LAS data to read and write LASzip-compressed data. The
BSD-licensed `libLAS`_ and the LGPL-licensed `LASlib`_ can take
advantage of LASzip to read and write compressed data.

LASzip is completely lossless. It compresses bulky LAS files into compact LAZ
files that are only 10-20 percent of the original size, accurately preserving
every single bit. For example, compressing and decompressing the LAS file 
lidar.las with `laszip.exe`_ as shown below results in lidar_copy.las that
is bit-identical to lidar.las. However, the small size of lidar.laz makes it
much easier to store, copy, transmit, or archive large amounts of LIDAR.

* laszip lidar.las lidar.laz
* laszip lidar.laz lidar_copy.las

LASzip compression can be many times smaller and many times faster than
`bz2`_, `gzip`_, and `rar`_ because it takes advantage of knowing what the
different bytes in a LAS file represent.

Another advantage of LASzip over generic compressors is that it allows you to
treat the compressed LAZ files just like standard LAS files and load them
directly from compressed form into your application without the need to
decompress and write out the data in an uncompressed form. The availability of
the two APIs `libLAS`_ and `lasLIB`_ with LASzip capability makes it easy to
add native LAZ support to your own software package.


Download LAZ data
------------------------------------------------------------------------------

* http://gisfiles.wm.edu/files/lidar/
* http://gisfiles.wm.edu/files/lidar/ESLiDAR/LiDAR_ClassifiedPointCloud/
* ftp://lidar.dnr.state.mn.us/data/raw/county/dodge/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/faribault/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/fillmore/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/freeborn/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/waseca/laz/
* http://liblas.org/samples/

.. toctree::
   :hidden: 
   
   data
   
.. _`OSGeo4W`: http://trac.osgeo.org/osgeo4w
.. _`Martin Isenburg`: http://www.cs.unc.edu/~isenburg
.. _`ASPRS LAS format`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html
.. _`LAS`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html
.. _`LGPL`: http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License
.. _`bz2`: http://en.wikipedia.org/wiki/Bzip2
.. _`gzip`: http://en.wikipedia.org/wiki/Gzip
.. _`rar`: http://en.wikipedia.org/wiki/Rar
.. _`LAStools`: http://lastools.org
.. _`libLAS`: http://liblas.org
.. _`lasLIB`: http://www.cs.unc.edu/~isenburg/lastools/download/laslib_README.txt
.. _`lasLIB source`: http://www.cs.unc.edu/~isenburg/lastools/download/laslib.zip
.. _`laszip.exe`: http://www.cs.unc.edu/~isenburg/lastools/
.. _`laszip`: http://www.cs.unc.edu/~isenburg/lastools/download/laslib.zip
