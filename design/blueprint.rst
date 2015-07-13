===== 
LASzip for LAS 1.4 (native extension)
===== 
a working document 
-------- 
This document summarizes the new features and the implementation detail that will be part of the "native extension" of the LASzip compressor to compress the new point types 6 to 10 of the LAS 1.4 specification. The current LASzip compressor can already handle (most) LAS 1.4 files via what is called the "LAS 1.4 compatibility mode" that was sponsored in part by NOAA, Trimble, and Quantum Spatial. However, the LAS 1.4 files are likely to stay with us for a long time. The "native extension" of LASzip to seeks to exploit the "natural break" in the lineage of the LAS specification to add entirely new features to the compressor that allow better exploitation of LAS files in the cloud or on Web portals.

In particular we invite ESRI to tell us what exactly they need from the new "native extension" of LASzip. ESRI has claimed to have identified very particular needs for processing LiDAR in the cloud that have warranted the development of a competing closed standard also know as the "LAZ clone". The resulting format fragmentation has upset the geospatial community and led to an open letter that has asked ESRI to change their course and work with the LiDAR users to avoid a format war. In parallel to the mediatation being under way via the Open Geospatial Consortium (OGC) we are intending to progress towards a set of useful features for compressing LAS 1.4 content in an open and transparent manner, yet at the speed that the needs of our industry demands.

New Features for the native LAS 1.4 extension
-------- 
**1. Selective Decompression:**

Often only a subset of all the attributes of a LiDAR return stored in the LAS format are actually needed by an application. Currently all points attributes have to be read and decompressed by an application opening a LAS or a LAZ file. This is the most far reaching new feature of the native LAS 1.4 extension of LASzip: for the new point types, it will allow reading and decompressing only those points attributes that are relevant to the application. A Web-based LiDAR viewer, for example, may  only have to download, parse, and decompress the xyz coordinates and the intensities of the point cloud and a cloud-based service for DTM generation will only have to download, parse, and decompress the xyz coordinates and the classification values.

**2. Re-writeable Flags and Classification**

Most of the point attribute of an LAS file will never change --- *at laest* once the LiDAR was published. The xyz coordinates are final once the LiDAR flightlines were aligned, the return counts are fix, the intensities are direct measured, and the original GPS time stamps and the point source IDs should be preserved for posterity. What often does change, however, are the point classifications into "ground", "building", "noise", or "water" points, as well as the flags describing which points are to be "withheld" or which ones are part of the not always desired "overlap".  This is the second far reaching new feature of the native LAS 1.4 extension of LASzip: based on mechanisms that have already been field-tested as part of the "LASlayers" effort the new LASzip will support overriding the existing classifications or flags with a new layer of values. 

**3. Full Integration of Spatial Indexing**

Already supported in the existing LASzip compressor as an optional item this will become a mandatory part of every new LAZ file that is written. Area-of-interest queries are also a form or "selective decompression" and require two things: Knowledge where in the file the points that fall into the interesting area are located (e.g. in the seventeen point intervals [5236312,5236312], [6523634,6634523], ....) and the ability to seek in the compressed file and decompress only those point intervals. The letter has been an integral part of the LASzip compressor since day one as this was one of the core features sponsored by USACE. The first has been added after little by little to LASzip in order not to disrupt anything since the concept (i.e. LASindex and tiny LAX files) was first introduced at ELMF in 2012. The concept needs to be reworked slightly to accommodate files with over 4 billion points.

**4. Attach-able Attributes (optional)**

Some LiDAR processing steps create additional per-point attributes such as RGB colors or NDVI values from a different source, error estimates, or point normals. During the design process of the the native LAS 1.4 extension of LASzip we want to consider to allow adding such attributes later without requiring a complete re-write of all existing points attributes that have not changed. 

**5. Explode-able Files (optional)**

Selective decompression - or more importantly selective download - of large files may in some cases be more feasible to implement for a 3D Web viewer or a LiDAR service portal when the data for selectable attributes is stored for download in separate files. During the design process of the the native LAS 1.4 extension of LASzip we want to accomodate to later add the option to store one compressed LAZ file as a number of compressed files each of which encodes a different set of point attributes.  

**6. Specification Document**

The LASzip compressor is currently only documented via an open source reference implementation in C++. In order to create LASzip compressors and decompressors in other programming languages it is currently necessary to step through the (reasonably well documented) C++ source code. We hope that funds can be made available that allow us to hire technical writers who can create a proper specification document that describes the open LASzip compressed LiDAR format.
