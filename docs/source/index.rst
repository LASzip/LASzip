.. _home:

******************************************************************************
LASzip
******************************************************************************


Current Release(s)
------------------------------------------------------------------------------

* **2011-02-01** 

  - `laszip-1.0.1-src.tar.gz <http://download.osgeo.org/laszip/laszip-1.0.1.tar.gz>`_ 
    `(md5) <http://download.osgeo.org/laszip/laszip-1.0.1.tar.gz.md5>`__ 

  - `laszip-1.0.1-src.zip <http://download.osgeo.org/laszip/laszip-1.0.0.zip>`_ 
    `(md5) <http://download.osgeo.org/liblas/laszip-1.0.1.zip.md5>`__

  - See `OSGeo4W <http://trac.osgeo.org/osgeo4w>`__ for Windows release.

  - Or try `laszip.exe`_ a very efficient standalone implementation based
    on the lighter-weight `LASlib`_ API used by `LAStools`_

Development Source
..............................................................................

* Current Trunk: http://hg.liblas.org/zip


Background
------------------------------------------------------------------------------

LASzip is a compression library that was developed by `Martin Isenburg`_ for 
compressing `ASPRS LAS format`_ data in his `LAStools`_.  
It has been provided as an `LGPL`_-licensed stand-alone software library to allow 
other softwares that handle LAS data to read and write LASzip-compressed data.  
libLAS can optionally take advantage of LASzip as of libLAS 1.6.0b4 to read 
and write compressed data. Currently, both LAStools and libLAS can interchange
data using LASzip compression.

The compression can be many times smaller and many times faster than `bz2`_,
`gzip`_, and `rar`_ because it takes advantage of knowing what the different
bytes in a LAS file represent. For example, the x and y coordinate of a
point are predicted by adding their delta in the preceding two points to 
the x and y of the last point, whereas the gps_time difference between
subsequent points is expressed as a multiple of the most common increment,
while RGB valued are compressed with simple difference coding. In each case
the prediction residuals are compressed with adaptive arithemetic coding.

The compressor treats the LAS points of types 0, 1, 2, 3, 4, 5, and 6  as 
onsisting of a number of items: POINT10, GPSTIME11, RGB12, and WAVEPACKET13
and uses separate modules with separate version numbers to compress each
item. This makes it easy to improve, for example, the compressor for the
gps_time without affecting the other compression schemes, while at the same
time guaranteeing full backwards compatability. More importantly it allows
to add new LAS point types as the LAS specification evolves into the future.

Another advantage of LASzip over generic compressors is that it allows you
to treat the compressed LAZ files just like standard LAS files and load them
directly from compressed form into your application without the need to ever
uncompress them points. The availability of reading and writing compressed
LAZ files via the two APIs `libLAS`_ and `lasLIB`_ makes it easy to add 
native LAZ support to your own software package.


.. _`Martin Isenburg`: http://www.cs.unc.edu/~isenburg
.. _`ASPRS LAS format`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html
.. _`LGPL`: http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License
.. _`bz2`: http://en.wikipedia.org/wiki/Bzip2
.. _`gzip`: http://en.wikipedia.org/wiki/Gzip
.. _`rar`: http://en.wikipedia.org/wiki/Rar
.. _`LAStools`: http://lastools.org
.. _`libLAS`: http://liblas.org
.. _`lasLIB`: http://www.cs.unc.edu/~isenburg/lastools/download/laslib.zip
.. _`laszip.exe`: http://www.cs.unc.edu/~isenburg/lastools/download/laszip.exe
