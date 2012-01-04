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

* **2012-01-04** 

  - `laszip-2.1.0.tar.gz <http://download.osgeo.org/laszip/laszip-2.1.0.tar.gz>`_ 
    `(md5) <http://download.osgeo.org/laszip/laszip-2.1.0.tar.gz.md5>`__ 

  - `laszip-2.1.0.zip <http://download.osgeo.org/laszip/laszip-2.1.0.zip>`_ 
    `(md5) <http://download.osgeo.org/liblas/laszip-2.1.0.zip.md5>`__


Binaries
..............................................................................

  - See `OSGeo4W`_ for Windows .lib and 
    include files to compile LASzip into your own application. `libLAS`_ 
    binaries through `OSGeo4W`_ also link LASzip.

  - For explicit compression (decompression) of LAS (LAZ) files a Windows
    binary `laszip.exe`_ is available, as well as the source code and a
    makefile to build `laszip`_ on Windows, Linux, or MacOS.


Documentation
..............................................................................

  - The LASzip `paper`_ and `video`_ from the ELMF 2011 presentation in Salzburg, Austria. 


Development Source
------------------------------------------------------------------------------

* Current Trunk: https://github.com/LASzip/LASzip


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
APIs, `libLAS`_ and `LASlib`_, with LASzip capability makes it very easy to
add native LAZ support to your own software package.

Download LAZ data
------------------------------------------------------------------------------

* http://opentopography.org/
* http://www.csc.noaa.gov/digitalcoast/data/chartstopobathy/download.html
* http://gisfiles.wm.edu/files/lidar/
* http://gisfiles.wm.edu/files/lidar/ESLiDAR/LiDAR_ClassifiedPointCloud/
* http://gisfiles.wm.edu/files/lidar/a11county/LAZ/
* ftp://lidar.dnr.state.mn.us/data/raw/county/dodge/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/douglas/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/faribault/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/fillmore/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/freeborn/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/houston/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/jackson/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/lesueur/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/martin/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/mower/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/murray/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/nobles/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/olmsted/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/pine/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/pope/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/rice/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/rock/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/steele/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/wabasha/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/waseca/laz/
* ftp://lidar.dnr.state.mn.us/data/raw/county/winona/laz/
* http://liblas.org/samples/

Users of LASzip
------------------------------------------------------------------------------

* `Blom <http://www.blomasa.com/>`_
* `Watershed Sciences, Inc. <http://www.watershedsciences.com/>`_
* `Riegl <http://www.riegl.com/>`_
* `NOAA <http://www.noaa.gov/>`_
* `USGS <http://www.usgs.gov/>`_
* `Dielmo 3D <http://www.dielmo.com/>`_
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
.. _`LASlib`: http://lastools.org/download/laslib_README.txt
.. _`LASlib source`: http://lastools.org/download/laslib.zip
.. _`laszip`: http://lastools.org/download/laszip.zip
.. _`laszip.exe`: http://lastools.org/
.. _`paper`: http://lastools.org/download/laszip.pdf
.. _`video`: http://www.youtube.com/watch?v=A0s0fVktj6U
