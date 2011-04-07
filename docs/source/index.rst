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

* **2011-04-05** 

  - `laszip-1.2.0-src.tar.gz <http://download.osgeo.org/laszip/laszip-1.2.0.tar.gz>`_ 
    `(md5) <http://download.osgeo.org/laszip/laszip-1.2.0.tar.gz.md5>`__ 

  - `laszip-1.2.0-src.zip <http://download.osgeo.org/laszip/laszip-1.2.0.zip>`_ 
    `(md5) <http://download.osgeo.org/liblas/laszip-1.2.0.zip.md5>`__

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
generic compressors like `bz2`_, `gzip`_, and `rar`_ because it knows what
the different bytes in a LAS file represent. Another advantage of LASzip
is that it allows you to treat compressed LAZ files just like standard LAS
files. You can load them directly from compressed form into your application 
without needing to decompress them onto disk first. The availability of two
APIs, `libLAS`_ and `lasLIB`_, with LASzip capability makes it very easy to
add native LAZ support to your own software package.

Download LAZ data
------------------------------------------------------------------------------

* http://gisfiles.wm.edu/files/lidar/
* http://gisfiles.wm.edu/files/lidar/ESLiDAR/LiDAR_ClassifiedPointCloud/
* ftp://lidar.dnr.state.mn.us/data/raw/county/dodge/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/douglas/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/faribault/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/fillmore/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/freeborn/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/houston/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/martin/las/
* ftp://lidar.dnr.state.mn.us/data/raw/county/mower/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/olmsted/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/pine/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/pope/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/rice/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/steele/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/wabasha/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/waseca/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/winona/laz/
* http://liblas.org/samples/

Users of LASzip
------------------------------------------------------------------------------

* `Riegl <http://www.riegl.com/>`_
* `TopoDOT <http://www.certainty3d.com/products/topodot/>`_
* `Oregon Lidar Consortium <http://www.oregongeology.org/sub/projects/olc/default.htm>`_
* `Minnesota Department of Natural Resources <http://www.dnr.state.mn.us/>`_
* `US Army Corps of Engineers <http://erdc.usace.army.mil/>`_

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
