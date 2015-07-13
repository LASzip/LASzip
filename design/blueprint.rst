===== 
LASzip for LAS 1.4 (native extension)
===== 
a working document 
-------- 
This document summarizes the new features and the implementation detail that will be part of the "native extension" of the LASzip compressor to compress the new point types 6 to 10 of the LAS 1.4 specification. The current LASzip compressor can already handle (most) LAS 1.4 files via what is called the "LAS 1.4 compatibility mode" that was sponsored in part by NOAA, Trimble, and Quantum Spatial. However, the LAS 1.4 files are likely to stay with us for a long time. The "native extension" of LASzip to seeks to exploit the "natural break" in the lineage of the LAS specification to add entirely new features to the compressor that allow better exploitation of LAS files in the cloud or on Web portals.

In particular we invite ESRI to tell us what exactly they need from the new "native extension" of LASzip. ESRI has claimed to have identified very particular needs for processing LiDAR in the cloud that have warranted the development of a competing closed standard also know as the "LAZ clone". The resulting format fragmentation has upset the geospatial community and led to an open letter that has asked ESRI to change their course and work with the LiDAR users to avoid a format war. In parallel to the mediatation being under way via the Open Geospatial Consortium (OGC) we are intending to progress towards a set of useful features for compressing LAS 1.4 content in an open and transparent manner, yet at the speed that the needs of our industry demands.
