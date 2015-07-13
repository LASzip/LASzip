===== 
LASzip for LAS 1.4 (native extension)
===== 
a working document 
-------- 
This document summarizes the new features and the implementation detail that will be part of the "native extension" of the LASzip compressor to compress the new point types 6 to 10 of the LAS 1.4 specification. The current LASzip compressor can already handle (most) LAS 1.4 files via what is called the "LAS 1.4 compatibility mode" that was sponsored in part by NOAA, Trimble, and Quantum Spatial. However, the LAS 1.4 files are likely to stay with us for a long time. The "native extension" of LASzip to seeks to exploit the "natural break" in the lineage of the LAS specification to add entirely new features to the compressor that allow better exploitation of LAS files in the cloud or on Web portals.

In particular we invite ESRI to tell us what exactly they need from the new "native extension" of LASzip. ESRI has claimed to have identified very particular needs for processing LiDAR in the cloud that have warranted the development of a competing closed standard also know as the "LAZ clone". The resulting format fragmentation has upset the geospatial community and led to an open letter that has asked ESRI to change their course and work with the LiDAR users to avoid a format war. In parallel to the mediatation being under way via the Open Geospatial Consortium (OGC) we are intending to progress towards a set of useful features for compressing LAS 1.4 content in an open and transparent manner, yet at the speed that the needs of our industry demands.

new feature for native LAS 1.4 extension
-------- 
**1. Selective Decompression:**

Often only a subset of all the attributes of a LiDAR return stored in the LAS format are actually needed by an application. Currently all points attributes have to be read and decompressed by an application opening a LAS or a LAZ file. This is the most far reaching new feature of the native LAS 1.4 extension of LASzip: for the new point types, it will allow reading and decompressing only those points attributes that are relevant to the application. A Web-based LiDAR viewer, for example, may  only have to download, parse, and decompress the xyz coordinates and the intensities of the point cloud and a cloud-based service for DTM generation will only have to download, parse, and decompress the xyz coordinates and the classification values.

2. re-writeable flags and classification

3. specification document
