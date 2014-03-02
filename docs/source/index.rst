.. _home:

******************************************************************************
LASzip - free and lossless LiDAR compression
******************************************************************************

LASzip - a free product of `rapidlasso GmbH <http://rapidlasso.com/>`_ - quickly turns bulky LAS files into compact LAZ files without information loss. Terabytes of LAZ data are now available for free download from various agencies making LASzip, winner of the 2012 Geospatial World Forum `Technology Innovation Award <http://www.facebook.com/photo.php?fbid=370334049695712>`_ in LiDAR Processing and 2nd place winner for `innovative product at INTERGEO 2012 <http://gispoint.de/wia.html>`_, the de-facto standard for LiDAR compression.


Download
------------------------------------------------------------------------------

The LPGP-licensed LASzip library is easiest integrated via the DLL that is in
the LASzip subdirectory of the `LAStools`_ distribtion. Other options are to
link LASzip via the BSD-licensed `libLAS`_ library or to work with the `LASlib`_
library which fully integrates and enhances the LASzip codebase with spatial
indexing, filters, transforms, geo-referencing, ... of LAS and LAZ files.


Source
..............................................................................


* **2013-08-05** 

  - `laszip-2.2.0.tar.gz <https://github.com/LASzip/LASzip/releases/download/v2.2.0/laszip-src-2.2.0.tar.gz>`_ 
    `(md5) <https://github.com/LASzip/LASzip/releases/download/v2.2.0/laszip-src-2.2.0.tar.gz.md5>`__ 

  - `laszip-win32-msvc2009.zip <https://github.com/LASzip/LASzip/releases/download/v2.2.0/laszip-win32-msvc2009.zip>`_ 
    `(md5) <https://github.com/LASzip/LASzip/releases/download/v2.2.0/laszip-win32-msvc2009.zip.md5>`__


Past Release(s)
~~~~~~~~~~~~~~~~~~~~~~~~~


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
    binary `laszip.exe`_ is available (command-line only `laszip-cli.exe`_),
    as well as source code, examples, and makefiles to build your
    own `LASzip`_ on Windows, Linux, or MacOS.


Documentation
..............................................................................

  - The LASzip `paper`_ and `video`_ from the ELMF 2011 presentation in Salzburg, Austria. An `interview <http://geodatapoint.com/articles/view/expanding_access_to_lidar_through_compression>`_ with editor Christine Grahl of POB magazine.


Development Source
------------------------------------------------------------------------------

* Current Trunk: https://github.com/LASzip/LASzip


Background
------------------------------------------------------------------------------

LASzip is a compression library that was developed by `Martin Isenburg`_ for
compressing `ASPRS LAS format`_ data in his `LAStools`_. It has been provided
as an `LGPL`_-licensed stand-alone software library to allow other softwares
that handle LAS data to read and write LASzip-compressed data. The BSD-licensed
`libLAS`_ and the LGPL-licensed `LASlib`_ can take advantage of LASzip to read
and write compressed data.

LASzip is completely lossless. It compresses bulky LAS files into compact LAZ
files that are only 7-20 percent of the original size, accurately preserving
every single bit. For example, compressing and decompressing the LAS file 
lidar.las with `laszip.exe`_ (command-line only `laszip-cli.exe`_) as shown below results in lidar_copy.las that
is bit-identical to lidar.las. However, the small size of lidar.laz makes it
much easier to store, copy, transmit, or archive large amounts of LIDAR.

* laszip -i lidar.las -o lidar.laz
* laszip -i lidar.laz -o lidar_copy.las

LASzip compression can be many times smaller and many times faster than
generic compressors like `bz2`_, `gzip`_, and `rar`_ because it knows what
the different bytes in a LAS file represent. Another advantage of LASzip
is that it allows you to treat compressed LAZ files just like standard LAS
files. You can load them directly from compressed form into your application 
without needing to decompress them onto disk first. The availability of the
`LASzip`_ DLL and two APIs, `libLAS`_ and `LASlib`_, with LASzip capability
makes it easy to add native LAZ support to your own software package.

Software with native LAZ support
------------------------------------------------------------------------------

* QT Modeler (7.1.6 and up) by `Applied Imagery <http://www.appliedimagery.com/>`_
* FUSION (3.40 and up) by `Bob McGaughey of the USDA <http://forsys.cfr.washington.edu/fusion/fusionlatest.html/>`_
* Global Mapper (13.1 and up) by `Blue Marble Geo <http://www.bluemarblegeo.com/>`_
* ENVI LiDAR (5.1 and up) by `Exelis VIS <http://www.exelisvis.com/ProductsServices/ENVI/ENVILiDAR.aspx>`_
* OCAD (11.4.0 and up) by `OCAD Inc. <http://ocad.com/blog/2013/11/ocad-11-4-0-supports-laz-files/>`_
* FME (2012 and up) by `Safe Software <http://www.safe.com/>`_
* TopoDOT by `Certainty3D <http://www.certainty3d.com/products/topodot/>`_
* Pointools by `Bentley Systems <http://www.pointools.com/>`_
* Pix4uav by `Pix4D <http://www.pix4d.com/>`_
* SURE by `nframes <http://www.nframes.com/>`_
* LAStools by `rapidlasso - fast tools to catch reality <http://rapidlasso.com/>`_
* RiProcess by `RIEGL LMS GmbH <http://www.riegl.com/>`_
* ZEB1 by `3D Laser Mapping <http://www.3dlasermapping.com/index.php/products/handheld-mapping>`_
* Gexcel R3 by `Gexcel <http://www.gexcel.it/en/software/>`_
* Voyager (1.3 and up) by `Voyager GIS <http://www.voyagergis.com/>`_
* Scanopy by `imagination <http://www.imagination.at/>`_
* ScanLook by `LiDAR USA <http://www.lidarusa.com/>`_
* GRASS GIS (7.0 and up) by `Open Source Geospatial Foundation (OSGeo) <http://grass.osgeo.org/>`_
* OPALS by `TU Vienna <http://www.ipf.tuwien.ac.at/opals/>`_
* ReportGen (2.9.0 and up), by `PDF3D <http://pdf3d.com/news/PDF3D_LiDAR_PressRelease_20dec2013.php>`_
* CloudCompare by `Daniel Girardeau-Montaut <http://www.danielgm.net/cc/>`_
* OrbitGIS by `Orbit <http://www.orbitgis.com/>`_
* K2Vi by `AAM Group <http://www.aamgroup.com/services-and-technology/3d-gis>`_
* LiS by `LASERDATA <http://www.laserdata.at/>`_
* Geoverse by `euclideon <http://www.euclideon.com/>`_
* PointCloudViz by `mirage <http://www.mirage-tech.com/>`_

Download LAZ data
------------------------------------------------------------------------------
* `Puget Sound LiDAR Consortium <http://pugetsoundlidar.ess.washington.edu/lidardata/>`_

  * 2007: `sumpter <http://pugetsoundlidar.ess.washington.edu/lidardata/restricted/las/pslc2007/sumpter/>`_

  * 2009: `douglasco <http://delta.ess.washington.edu/lazfiles/pslc2009/douglasco/>`_, `snohoriver <http://delta.ess.washington.edu/lazfiles/pslc2009/snohoriver/>`_, `umpqua <http://delta.ess.washington.edu/lazfiles/pslc2009/umpqua/>`_, `wenas <http://delta.ess.washington.edu/lazfiles/pslc2009/wenas/>`_, `wenatchee <http://delta.ess.washington.edu/lazfiles/pslc2009/wenatchee/>`_

  * 2011: `kittitas <http://delta.ess.washington.edu/lazfiles/pslc2011/kittitas/>`_, `quinault <http://delta.ess.washington.edu/lazfiles/pslc2011/quinault/>`_, `rattlesnake <http://delta.ess.washington.edu/lazfiles/pslc2011/rattlesnake/>`_

  * 2012: `chehalis <http://pugetsoundlidar.ess.washington.edu/lidardata/restricted/las/pslc2012/chehalis/>`_, `hoh <http://pugetsoundlidar.ess.washington.edu/lidardata/restricted/las/pslc2012/hoh/>`_, `jefferson_clallam <http://pugetsoundlidar.ess.washington.edu/lidardata/restricted/las/pslc2012/jefferson_clallam/>`_, `quinault <http://pugetsoundlidar.ess.washington.edu/lidardata/restricted/las/pslc2012/usgs_quinault/>`_, `upper_naches <http://pugetsoundlidar.ess.washington.edu/lidardata/restricted/las/pslc2012/upper_naches/>`_

* open LiDAR data strategy of the `National Land Survey of Finland <https://tiedostopalvelu.maanmittauslaitos.fi/tp/kartta?lang=en>`_
* `Digital Coast LiDAR <https://www.csc.noaa.gov/digitalcoast/data/coastallidar>`_ by NOAA (+ `how to download <http://www.csc.noaa.gov/digitalcoast/geozone/how-download-lots-lidar-digital-coast>`_)

  * Batch Download Folders: `12 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/12/>`_, `13 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/13/>`_, `15 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/15/>`_, `16 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/16/>`_, `17 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/17/>`_, `18 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/18/>`_, `19 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/19/>`_, `20 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/20/>`_, `23 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/23/>`_, `24 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/24/>`_, `25 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/25/>`_, `26 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/26/>`_, `27 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/27/>`_, `28 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/28/>`_, `29 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/29/>`_, `31 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/31/>`_, `32 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/32/>`_, `33 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/33/>`_, `34 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/34/>`_, `35 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/35/>`_, `36 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/36/>`_, `37 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/37/>`_, `39 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/39/>`_, `40 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/40/>`_, `41 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/41/>`_, `44 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/44/>`_, `45 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/45/>`_, `46 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/46/>`_, `47 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/47/>`_, `48 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/48/>`_, `49 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/49/>`_, `50 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/50/>`_, `51 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/51/>`_, `52 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/52/>`_, `53 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/53/>`_, `54 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/54/>`_, `55 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/55/>`_, `56 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/56/>`_, `57 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/57/>`_, `62 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/62/>`_, `63 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/63/>`_, `64 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/64/>`_, `65 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/65/>`_, `66 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/66/>`_, `67 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/67/>`_, `68 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/68/>`_, `69 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/69/>`_, `71 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/71/>`_, `76 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/76/>`_, `77 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/77/>`_, `78 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/78/>`_, `79 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/79/>`_, `81 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/81/>`_, `82 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/82/>`_, `83 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/83/>`_, `84 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/84/>`_, `85 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/85/>`_, `86 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/86/>`_, `87 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/87/>`_, `88 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/88/>`_, `89 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/89/>`_, `90 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/90/>`_, `91 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/91/>`_, `92 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/92/>`_, `93 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/93/>`_, `94 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/94/>`_, `95 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/95/>`_, `96 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/96/>`_, `97 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/97/>`_, `98 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/98/>`_, `99 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/99/>`_, `100 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/100/>`_, `101 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/101/>`_, `102 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/102/>`_, `103 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/103/>`_, `104 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/104/>`_, `105 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/105/>`_, `106 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/106/>`_, `107 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/107/>`_, `108 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/108/>`_, `109 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/109/>`_, `111 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/111/>`_, `112 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/112/>`_, `114 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/114/>`_, `115 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/115/>`_, `116 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/116/>`_, `117 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/117/>`_, `501 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/501/>`_, `502 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/502/>`_, `503 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/503/>`_, `504 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/504/>`_, `505 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/505/>`_, `506 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/506/>`_, `507 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/507/>`_, `508 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/508/>`_, `509 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/509/>`_, `510 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/510/>`_, `511 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/511/>`_, `512 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/512/>`_, `513 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/513/>`_, `514 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/514/>`_, `515 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/515/>`_, `516 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/516/>`_, `517 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/517/>`_, `518 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/518/>`_, `519 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/519/>`_, `520 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/520/>`_, `521 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/521/>`_, `522 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/522/>`_, `523 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/523/>`_, `524 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/524/>`_, `525 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/525/>`_, `526 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/526/>`_, `527 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/527/>`_, `528 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/528/>`_, `529 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/529/>`_, `530 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/530/>`_, `531 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/531/>`_, `532 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/532/>`_, `533 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/533/>`_, `534 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/534/>`_, `535 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/535/>`_, `536 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/536/>`_, `537 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/537/>`_, `538 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/538/>`_, `539 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/539/>`_, `540 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/540/>`_, `541 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/541/>`_, `542 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/542/>`_, `543 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/543/>`_, `544 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/544/>`_, `545 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/545/>`_, `546 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/546/>`_, `547 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/547/>`_, `548 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/548/>`_, `549 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/549/>`_, `550 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/550/>`_, `551 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/551/>`_, `552 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/552/>`_, `553 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/553/>`_, `554 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/554/>`_, `555 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/555/>`_, `556 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/556/>`_, `557 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/557/>`_, `558 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/558/>`_, `559 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/559/>`_, `560 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/560/>`_, `561 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/561/>`_, `562 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/562/>`_, `563 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/563/>`_, `564 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/564/>`_, `565 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/565/>`_, `566 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/566/>`_, `567 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/567/>`_, `568 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/568/>`_, `569 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/569/>`_, `570 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/570/>`_, `571 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/571/>`_, `572 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/572/>`_, `573 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/573/>`_, `574 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/574/>`_, `575 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/575/>`_, `576 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/576/>`_, `577 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/577/>`_, `578 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/578/>`_, `579 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/579/>`_, `581 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/581/>`_, `582 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/582/>`_, `583 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/583/>`_, `584 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/584/>`_, `586 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/586/>`_, `587 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/587/>`_, `588 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/588/>`_, `1061 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1061/>`_, `1063 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1063/>`_, `1064 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1064/>`_, `1065 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1065/>`_, `1066 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1066/>`_, `1069 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1069/>`_, `1070 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1070/>`_, `1071 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1071/>`_, `1072 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1072/>`_, `1073 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1073/>`_, `1076 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1076/>`_, `1077 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1077/>`_, `1078 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1078/>`_, `1079 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1079/>`_, `1080 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1080/>`_, `1117 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1117/>`_, `1118 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1118/>`_, `1119 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1119/>`_, `1120 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1120/>`_, `1121 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1121/>`_, `1122 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1122/>`_, `1123 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1123/>`_, `1124 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1124/>`_, `1125 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1125/>`_, `1132 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1132/>`_, `1133 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1133/>`_, `1158 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1158/>`_, `1159 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1159/>`_, `1170 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1170/>`_, `1171 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1171/>`_, `1172 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1172/>`_, `1174 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1174/>`_, `1175 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1175/>`_, `1176 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1176/>`_, `1178 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1178/>`_, `1179 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1179/>`_, `1198 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1198/>`_, `1199 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1199/>`_, `1381 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1381/>`_, `1382 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1382/>`_, `1389 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1389/>`_, `1390 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1390/>`_, `1391 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1391/>`_, `1392 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1392/>`_, `1393 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1393/>`_, `1397 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1397/>`_, `1398 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1398/>`_, `1399 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1399/>`_, `1403 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1403/>`_, `1404 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1404/>`_, `1406 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1406/>`_, `1407 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1407/>`_, `1408 <http://www.csc.noaa.gov/htdata/lidar1_z/geoid12a/data/1408/>`_

* GRAFCAN `LiDAR of the Canary Islands <http://tiendavirtual.grafcan.es/visor.jsf?currentSeriePk=263585792>`_ 
* Alaska LiDAR in the `Matanuska-Susitna Borough <http://matsu.gina.alaska.edu/LiDAR/Point_MacKenzie/Point_Cloud/Classified.laz/>`_
* NSF-funded LiDAR hosting facility `OpenTopography <http://opentopo.sdsc.edu/gridsphere/gridsphere?cid=geonlidar>`_
* Clark County Kentucky LiDAR data `Clark County GIS <http://www.ccgisonline.com/LAZ/IndexMap.pdf>`_
* Nevada Barringer Meteorite Crater of the `Lunar and Planetary Institute <http://www.lpi.usra.edu/publications/books/barringer_crater_guidebook/LiDAR/>`_
* Alaska LiDAR hosted by `Division of Geological and Geophysical Surveys (DGGS) <http://maps.dggs.alaska.gov/lidar/#-16434084:9589812:5>`_

* Statewide LiDAR of `Elevation Mapping Project by Minnesota DNR <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/>`_

  * Counties: `aitkin <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/aitkin/laz/>`_, `anoka <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/anoka/laz/>`_, `becker <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/becker/laz/>`_, `beltrami <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/beltrami/laz/>`_, `benton <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/benton/laz/>`_, `bigstone <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/bigstone/laz/>`_, `blueearth <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/blueearth/2012/laz/>`_, `brown <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/brown/laz/>`_, `carlton <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/carlton/laz/>`_, `carver <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/carver/laz/>`_, `cass <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/cass/laz/>`_, `chippewa <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/chippewa/laz/>`_, `chisago <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/chisago/laz/>`_, `clay <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/clay/laz/>`_, `clearwater <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/clearwater/laz/>`_, `cook <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/cook/laz/>`_, `cottonwood <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/cottonwood/laz/>`_, `crowwing <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/crowwing/laz/>`_, `dakota <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/dakota/laz/>`_, `dodge <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/dodge/laz/>`_, `douglas <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/douglas/laz/>`_, `faribault <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/faribault/laz/>`_, `fillmore <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/fillmore/laz/>`_, `freeborn <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/freeborn/laz/>`_, `goodhue <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/goodhue/laz/>`_, `grant <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/grant/laz/>`_, `hennepin <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/hennepin/laz/>`_, `houston <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/houston/laz/>`_, `hubbard <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/hubbard/laz/>`_, `isanti <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/isanti/laz/>`_, `itasca <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/itasca/laz/>`_, `jackson <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/jackson/laz/>`_, `kanabec <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/kanabec/laz/>`_, `kandiyohi <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/kandiyohi/laz/>`_, `kittson <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/kittson/laz/>`_, `koochiching <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/koochiching/laz/>`_, `lacquiparle <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lacquiparle/laz/>`_, `lake <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lake/laz/>`_, `lakeofthewoods <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lakeofthewoods/laz/>`_, `lesueur <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lesueur/laz/>`_, `lincoln <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lincoln/laz/>`_, `lyon <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/lyon/laz/>`_, `mahnomen <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/mahnomen/laz/>`_, `marshall <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/marshall/laz/>`_, `martin <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/martin/laz/>`_, `meeker <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/meeker/laz/>`_, `millelacs <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/millelacs/laz/>`_, `morrison <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/morrison/laz/>`_, `mower <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/mower/laz/>`_, `murray <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/murray/laz/>`_, `nicollet <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/nicollet/laz/>`_, `nobles <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/nobles/laz/>`_, `norman <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/norman/laz/>`_, `olmsted <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/olmsted/laz/>`_, `ottertail <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/ottertail/laz/>`_, `pennington <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/pennington/laz/>`_, `pine <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/pine/laz/>`_, `pipestone <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/pipestone/laz/>`_, `polk <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/polk/laz/>`_, `pope <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/pope/laz/>`_, `ramsey <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/ramsey/laz/>`_, `redlake <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/redlake/laz/>`_, `redwood <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/redwood/laz/>`_, `renville <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/renville/laz/>`_, `rice <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/rice/laz/>`_, `rock <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/rock/laz/>`_, `roseau <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/roseau/laz/>`_, `scott <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/scott/laz/>`_, `sherburne <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/sherburne/laz/>`_, `sibley <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/sibley/laz/>`_, `stearns <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/stearns/laz/>`_, `steele <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/steele/laz/>`_, `stevens <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/stevens/laz/>`_, `swift <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/swift/laz/>`_, `todd <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/todd/laz/>`_, `traverse <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/traverse/laz/>`_, `wabasha <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/wabasha/laz/>`_, `wadena <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/wadena/laz/>`_, `waseca <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/waseca/laz/>`_, `washington <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/washington/laz/>`_, `watonwan <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/watonwan/laz/>`_, `wilkin <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/wilkin/laz/>`_, `winona <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/winona/laz/>`_, `wright <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/wright/laz/>`_, `yellowmedicine <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/county/yellowmedicine/laz/>`_

  * Arrowhead: `block 1 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_1/laz/>`_, `block 2 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_2/laz/>`_, `block 3 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_3/laz/>`_, `block 4 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_4/laz/>`_, `block 5 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/arrowhead/block_5/laz/>`_

  * Twin Cities Metro: `block a c <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_a_c/laz/>`_, `block b <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_b/laz/>`_, `block d <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_d/laz/>`_, `block e <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_e/laz/>`_, `block f <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_f/laz/>`_, `block g <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_g/laz/>`_, `block h <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_h/laz/>`_, `block dakota <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_dakota/laz/>`_, `block maple grove <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_maple_grove/laz/>`_, `block metro <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/metro/block_metro/laz/>`_

  * Central Lakes: `block 1 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/central_lakes/block_1/laz/>`_, `block 2 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/central_lakes/block_2/laz/>`_, `block 3 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/central_lakes/block_3/laz/>`_, `block 4 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/central_lakes/block_4/laz/>`_, `block 5 <ftp://ftp.lmic.state.mn.us/pub/data/elevation/lidar/projects/central_lakes/block_5/laz/>`_

* TERN AusCover Airborne LiDAR: `tumbarumba <http://tern-auscover.science.uq.edu.au/thredds/catalog/auscover/field_and_airborne_datasets/airborne/lidar/tumbarumba/catalog.html>`_, `tsliig <http://tern-auscover.science.uq.edu.au/thredds/catalog/auscover/field_and_airborne_datasets/airborne/lidar/tsliig/catalog.html>`_, `robsons creek <http://tern-auscover.science.uq.edu.au/thredds/catalog/auscover/field_and_airborne_datasets/airborne/lidar/robsons_creek/catalog.html>`_, `credo <http://tern-auscover.science.uq.edu.au/thredds/catalog/auscover/field_and_airborne_datasets/airborne/lidar/credo/catalog.html>`_, `chowilla <http://tern-auscover.science.uq.edu.au/thredds/catalog/auscover/field_and_airborne_datasets/airborne/lidar/chowilla/catalog.html>`_

* http://liblas.org/samples/
* `Virginia LiDAR <http://www.wm.edu/as/cga/Data%20Services/VALIDAR/index.php>`_ of College of William and Mary: `Eleven Coastal Counties <http://gisfiles.wm.edu/files/lidar/a11county/LAZ/>`_, `Eastern Shore <http://gisfiles.wm.edu/files/lidar/ESLiDAR/LiDAR_ClassifiedPointCloud/>`_, `Shenandoah Valley <http://gisfiles.wm.edu/files/lidar/SV/laz/>`_
* `MassGIS LiDAR Terrain Data <http://www.mass.gov/anf/research-and-tech/it-serv-and-support/application-serv/office-of-geographic-information-massgis/datalayers/lidar.html>`_
* https://www.lidar-online.com/product-list.php

Users of LASzip
------------------------------------------------------------------------------

* `Fugro <http://www.fugro.com/>`_
* `Blom <http://www.blomasa.com/>`_
* `COWI <http://www.cowi.dk/>`_
* `Watershed Sciences, Inc. <http://www.watershedsciences.com/>`_
* `Riegl <http://www.riegl.com/>`_
* `NOAA <http://www.noaa.gov/>`_
* `USGS <http://www.usgs.gov/>`_
* `Euclideon <http://www.euclideon.com/>`_
* `Dielmo 3D <http://www.dielmo.com/>`_
* `Oregon Lidar Consortium <http://www.oregongeology.org/sub/projects/olc/default.htm>`_
* `Minnesota Department of Natural Resources <http://www.dnr.state.mn.us/>`_
* `US Army Corps of Engineers <http://erdc.usace.army.mil/>`_
* and many many more ...

Download LAS data (yet to be laszipped)
------------------------------------------------------------------------------

* `Nationwide LiDAR Data of Denmark <http://download.kortforsyningen.dk/>`_
* `Spring 2011 Rhode Island Statewide LiDAR Data <http://www.edc.uri.edu/rigis/data/download/lidar/2011USGS/tiles.aspx/>`_
* `PAMAP - LiDAR data of Pennsylvania <ftp://pamap.pasda.psu.edu/pamap_lidar/>`_
* `Iowa LiDAR Mapping Project <http://geotree2.geog.uni.edu/IowaLidar/>`_
* `Sacramento-San Joaquin Delta LiDAR <ftp://atlas.ca.gov/pub/delta-vision/lidar2009/>`_
* `Illinois Height Modernization Project LiDAR <http://www.isgs.illinois.edu/nsdihome/webdocs/ilhmp/data.html>`_
* `Northwest Florida Water Management District LiDAR <http://www.nwfwmdlidar.com/>`_
* Washington University in St. Louis: `Franklin <ftp://lidar.wustl.edu/from_WRC/Franklin/LAS_Files/>`_, `Jasper <ftp://lidar.wustl.edu/from_WRC/Jasper/Lidar_Photoscience/JasperCounty_Classified_LAS/>`_, `Washington Iron <ftp://lidar.wustl.edu/from_WRC/Washington_Iron/LAS_Files/>`_, `St. Francois <ftp://lidar.wustl.edu/from_WRC/St_Francois/LAS_Files/>`_, `Jefferson City (1) <ftp://lidar.wustl.edu/from_WRC/Cole_Callaway_Osage/08232010/JeffersonCity/Classified_LAS/>`_, `Jefferson City (2) <ftp://lidar.wustl.edu/from_WRC/Cole_Callaway_Osage/08272010/JeffersonCity/Classified_LAS/>`_, `Jefferson City (3) <ftp://lidar.wustl.edu/from_WRC/Cole_Callaway_Osage/09012010/JeffersonCity_Classified_LAS/>`_, `Jefferson Ste Genevieve <ftp://lidar.wustl.edu/from_WRC/Jefferson_Ste_Genevieve/LAS_Files/>`_, `USGS Drive <ftp://lidar.wustl.edu/USGS_Drive/BE_LIDAR/>`_, `Stone County MO <ftp://lidar.wustl.edu/Stone/Stone_County_MO_Classified_LAS_G10PD00579/>`_
* Alaska LiDAR: `Caswell Lakes <http://matsu.gina.alaska.edu/LiDAR/Caswell_Lakes/Point_Cloud/Classified/>`_, `Core Area <http://matsu.gina.alaska.edu/LiDAR/Core_Area/Point_Cloud/Classified/>`_, `Matanuska <http://matsu.gina.alaska.edu/LiDAR/Matanuska/Point_Cloud/Classified/>`_, `North Susitna <http://matsu.gina.alaska.edu/LiDAR/North_Susitna/Point_Cloud/Classified/>`_, `Point MacKenzie <http://matsu.gina.alaska.edu/LiDAR/Point_MacKenzie/Point_Cloud/Classified/>`_, `Willow <http://matsu.gina.alaska.edu/LiDAR/Willow/Point_Cloud/Classified/>`_, `Talkeetna <http://matsu.gina.alaska.edu/LiDAR/Talkeetna/Point_Cloud/Classified/>`_
* Spain: `Pais Vasco LiDAR <ftp://ftp.geo.euskadi.net/lidar/LIDAR_2012_ETRS89/LAS/>`_

.. toctree::
   :hidden: 
   
.. _`OSGeo4W`: http://trac.osgeo.org/osgeo4w
.. _`Martin Isenburg`: http://www.cs.unc.edu/~isenburg
.. _`ASPRS LAS format`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html
.. _`LAS`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html
.. _`LGPL`: http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License
.. _`bz2`: http://en.wikipedia.org/wiki/Bzip2
.. _`gzip`: http://en.wikipedia.org/wiki/Gzip
.. _`rar`: http://en.wikipedia.org/wiki/Rar
.. _`LAStools`: http://lastools.org/download/lastools.zip
.. _`libLAS`: http://liblas.org
.. _`LASlib`: http://lastools.org/download/laslib.zip
.. _`LASzip`: http://lastools.org/download/laszip.zip
.. _`laszip.exe`: http://lastools.org/download/laszip.exe
.. _`laszip-cli.exe`: http://lastools.org/download/laszip-cli.exe
.. _`paper`: http://lastools.org/download/laszip.pdf
.. _`video`: http://www.youtube.com/watch?v=A0s0fVktj6U
