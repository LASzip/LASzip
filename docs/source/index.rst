.. _home:

******************************************************************************
LASzip - lossless LAS compression
******************************************************************************

Winner of Geospatial World Forum 2012 Technology Innovation Award in LiDAR Processing.

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

Software with native LAZ support
------------------------------------------------------------------------------
* TopoDOT by `Certainty3D <http://www.certainty3d.com/products/topodot/>`_
* FME 2012 by `Safe Software <http://www.safe.com/>`_
* Global Mapper 13.1 by `Blue Marble Geo <http://www.bluemarblegeo.com/>`_
* QT Modeler 7.1.6 by `Applied Imagery <http://www.appliedimagery.com/>`_
* Voyager 1.3 by `Voyager GIS <http://www.voyagergis.com/>`_
* LAStools by `rapidlasso - fast tools to catch reality <http://rapidlasso.com/>`_
* OPALS by `TU Vienna <http://www.ipf.tuwien.ac.at/opals/>`_

Download LAZ data
------------------------------------------------------------------------------
* open LiDAR data strategy of the `National Land Survey of Finland <https://tiedostopalvelu.maanmittauslaitos.fi/tp/kartta?lang=en>`_
* Digital Coast data by `NOAA <http://www.csc.noaa.gov/digitalcoast/data/chartstopobathy/download.html>`_
* LiDAR and Imagery Project by the `Matanuska-Susitna Borough <http://matsu.gina.alaska.edu/LiDAR/Point_MacKenzie/Point_Cloud/Classified.laz/>`_
* NSF-funded LiDAR hosting facility `OpenTopography <http://opentopo.sdsc.edu/gridsphere/gridsphere?cid=geonlidar>`_
* Statewide LiDAR of `Elevation Mapping Project by Minnesota DNR <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/>`_
  * counties: `brown <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/brown/laz/>`_, `chippewa <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/chippewa/laz/>`_, `chisago <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/chisago/laz/>`_, `cottonwood <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/cottonwood/laz/>`_, `crowwing <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/crowwing/laz/>`_, `dodge <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/dodge/laz/>`_, `douglas <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/douglas/laz/>`_, `faribault <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/faribault/laz/>`_, `fillmore <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/fillmore/laz/>`_, `freeborn <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/freeborn/laz/>`_, `houston <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/houston/laz/>`_, `jackson <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/jackson/laz/>`_, `kandiyohi <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/kandiyohi/laz/>`_, `kandiyohi <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/kandiyohi/laz/>`_, `lesueur <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lesueur/laz/>`_, `lincoln <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lincoln/laz/>`_, `lyon <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lyon/laz/>`_, `martin <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/martin/laz/>`_, `meeker <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/meeker/laz/>`_, `mower <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/mower/laz/>`_, `murray <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/murray/laz/>`_, `nicollet <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/nicollet/laz/>`_, `nobles <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/nobles/laz/>`_, `olmsted <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/olmsted/laz/>`_, `pine <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/pine/laz/>`_, `pipestone <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/pipestone/laz/>`_, `pope <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/pope/laz/>`_, `redwood <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/redwood/laz/>`_, `renville <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/renville/laz/>`_, `rice <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/rice/laz/>`_, `rock <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/rock/laz/>`_, `sibley <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/sibley/laz/>`_, `stearns <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/stearns/laz/>`_, `steele <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/steele/laz/>`_, `swift <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/swift/laz/>`_, `wabasha <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/wabasha/laz/>`_, `waseca <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/waseca/laz/>`_, `watonwan <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/watonwan/laz/>`_, `winona <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/winona/laz/>`_, `wright <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/wright/laz/>`_, `yellowmedicine <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/yellowmedicine/laz/>`_
  * projects: `arrowhead (block 1) <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_1/laz/>`_, `arrowhead (block 2) <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_2/laz/>`_, `arrowhead (block 3) <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_3/laz/>`_, `arrowhead (block 4) <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_4/laz/>`_, `arrowhead (block 5) <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_5/laz/>`_
* http://liblas.org/samples/
* https://www.lidar-online.com/product-list.php
* http://gisfiles.wm.edu/files/lidar/
* http://gisfiles.wm.edu/files/lidar/ESLiDAR/LiDAR_ClassifiedPointCloud/
* http://gisfiles.wm.edu/files/lidar/a11county/LAZ/

Users of LASzip
------------------------------------------------------------------------------

* `Fugro <http://www.fugro.com/>`_
* `Blom <http://www.blomasa.com/>`_
* `Watershed Sciences, Inc. <http://www.watershedsciences.com/>`_
* `Riegl <http://www.riegl.com/>`_
* `NOAA <http://www.noaa.gov/>`_
* `USGS <http://www.usgs.gov/>`_
* `Dielmo 3D <http://www.dielmo.com/>`_
* `Oregon Lidar Consortium <http://www.oregongeology.org/sub/projects/olc/default.htm>`_
* `Minnesota Department of Natural Resources <http://www.dnr.state.mn.us/>`_
* `US Army Corps of Engineers <http://erdc.usace.army.mil/>`_
* ...

Download LAS data (yet to be laszipped)
------------------------------------------------------------------------------

* `PAMAP - LiDAR data of Pennsylvania <ftp://pamap.pasda.psu.edu/pamap_lidar/>`_
* `Puget Sound LiDAR Consortium <http://pugetsoundlidar.ess.washington.edu/lidardata/>`_
* `Iowa LiDAR Mapping Project <http://geotree2.geog.uni.edu/IowaLidar/>`_
* `Sacramento-San Joaquin Delta LiDAR <ftp://atlas.ca.gov/pub/delta-vision/lidar2009/>`_
* `Illinois Height Modernization Project LiDAR <http://www.isgs.illinois.edu/nsdihome/webdocs/ilhmp/data.html>`_
* `Utah Salt Lake County LiDAR <http://mapserv.utah.gov/rasterindicies/lidar_raw.html>`_

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
.. _`laszip.exe`: http://lastools.org/download/laszip.exe
.. _`paper`: http://lastools.org/download/laszip.pdf
.. _`video`: http://www.youtube.com/watch?v=A0s0fVktj6U
