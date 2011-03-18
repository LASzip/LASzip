.. _home:

******************************************************************************
LASzip
******************************************************************************

Download
------------------------------------------------------------------------------



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

  - A simple Windows binary, `laszip.exe`_, is also available for 
    explicit compression and decompression tasks to and from `ASPRS LAS format`_.
    

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
different bytes in a LAS file represent. For example, the ``x`` and ``y``
coordinate of a point are predicted by adding their delta in the preceding two
points to the ``x`` and ``y`` of the last point, whereas the difference
between subsequent points' time values is expressed as a multiple of the most
common increment, while RGB valued are compressed with simple difference
coding. In each case the prediction residuals are compressed with adaptive
arithmetic coding.

The compressor treats the LAS points of types 0, 1, 2, 3, 4, 5, and 6 as
consisting of a number of items: POINT10, GPSTIME11, RGB12, and WAVEPACKET13
and uses separate modules with separate version numbers to compress each item.
This makes it easy to improve, for example, the compressor for the
``gps_time`` without affecting the other compression schemes while
guaranteeing full backwards compatibility. More importantly it allows the
engine to add new LAS point types as the LAS specification evolves into the
future without affecting existing point types.

Another advantage of LASzip over generic compressors is that it allows you to
treat the compressed LAZ files just like standard LAS files and load them
directly from compressed form into your application without the need to
decompress and write out the data in an uncompressed form. The availability of
the two APIs `libLAS`_ and `lasLIB`_ with LASzip capability makes it easy to
add native LAZ support to your own software package.

.. note::
    `libLAS`_ externally links to released forms of the library at 
    http://laszip.org while `LASlib`_ currently embeds this same 
    compression/decompression codebase.

Data
------------------------------------------------------------------------------

LASzip-compressed data is available from :ref:`here <data>`.


.. toctree::
   :hidden: 
   
   data
   
.. _`OSGeo4W`: http://trac.osgeo.org/osgeo4w
.. _`Martin Isenburg`: http://www.cs.unc.edu/~isenburg
.. _`ASPRS LAS format`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html
.. _`LGPL`: http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License
.. _`bz2`: http://en.wikipedia.org/wiki/Bzip2
.. _`gzip`: http://en.wikipedia.org/wiki/Gzip
.. _`rar`: http://en.wikipedia.org/wiki/Rar
.. _`LAStools`: http://lastools.org
.. _`libLAS`: http://liblas.org
.. _`lasLIB`: http://www.cs.unc.edu/~isenburg/lastools/download/laslib_README.txt
.. _`lasLIB source`: http://www.cs.unc.edu/~isenburg/lastools/download/laslib.zip
.. _`laszip.exe`: http://www.cs.unc.edu/~isenburg/lastools/
