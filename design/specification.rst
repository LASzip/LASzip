As before the LiDAR points are compressed in completely independent chunks that default to 50,000 points per chunk. Each chunk can be decompressed "on its own" once the start byte is know. As before the first point of each chunk is stored raw. The attributes of all following points are broken into several layers. Only the first layer containing the x and y coordinates (and a few pieces of information) is mandatory to be read  mandartory. The remaining layers containing independently useful attributes such as elevation, intensity, GPS times, colors, point source ID, classifications and flags do not necessarily need to be read from disk and be decompressed.

Another new (or rather "revived") feature in LASzip compression for the new point types of LAS 1.4 is that it will be possible to continously vary the chunk size. This in the design of the original LASzip but broke when the file was streamed due to missing access to the per chunk point counts during a streaming read as those were only stored in the chunk table at the end of the LAZ file. This will be fixed by also storing the number of points inside each chunk.

LAZ File Layout:
----------------

1. LASheader
2. LASzip VLR
3. Sequence of n Chunks
3.1. Chunk 1
3.2. Chunk 2
3.3. Chunk 3
....
3.n. Chunk n
4. Chunk Table
5. EVLRs
6. LASindex EVLR
7. LASlayers EVLR (future feature: for in-place edits of classifications / flags)

Chunk Layout:
-------------
1) Raw point [30 - 68 (or more) bytes]
2) Numbers and Bytes
  + Number of remaining points [4 bytes]
  + Number of bytes (maybe compressed)
     - XYZ and return type layer [4 bytes]
     - classification and flags layer [4 bytes]
     - intensity layer [4 bytes]
     - point source ID layer [4 bytes]
     - scan angle layer [4 bytes]
     - user data layer [4 bytes]
     - GPS time stamp layer [4 bytes]
     - RGB layer [4 bytes]
     - NIR layer [4 bytes]
     - WavePacket layer [4 bytes]
     - "Extra Bytes" layer [4 bytes]
3) Layers
     - XYZ and return type layer
     - classification and flags layer
     - intensity layer
     - point source ID layer
     - scan angle layer
     - user data layer
     - GPS time stamp layer
     - RGB layer
     - NIR layer
     - WavePacket layer
     - "Extra Bytes" layer

Compression of XYZ and return layer
-----------------------------------
Due to the new scanner channel it is *crucial* to first encode whether a point is from the same and if not from which other scanner channel so that the correct context can be used for all subsequent predictions. Because of the strong correlation between GPS times, return numbers and number of returns awith each other and with the differences in XY we include them here as well.

* scanner channel different from previous point (yes / no)
* GPS time stamp different from previous point of same scanner channel (yes / no)
* number of returns different of same scanner channel (yes / no)
* return number different of same scanner channel (yes / no)

This is turned into one symbol of 4 bits that we compress with either with four or six different contexts

* 4 = based on whether the previous point is single or first/intermediate/last of many.
* 6 = based on whether the previous point is single, first/last of two, or first/intermediate/last of many.

If the scanner channel is different we compress this 2 bit symbol using the previous scanner channel one of four different contexts. Any following XYZ and attribute prediction based on a previous point is considering the previous point from the *same* scanner channel when such a point exists.

If the number of returns is different we compress it with the previous number of returns (of same scanner channel) as one of sixteen contexts.

If the return number different is different we compress it with the previous return number (of same scanner channel) and the information whether the GPS time stamp is different as one of thirtytwo contexts.

We compress X, Y, and Z similar to how LASzip does it currently (but make all predictions from the previous points from the same scanner channel).

Compression of classification and flags layer
---------------------------------------------
Compress the classification based on the context m and the previous value similar to the existing LASzip.

Compression of intensity layer
------------------------------
Compress the intensity based on the context m similar to the existing LASzip.

Compression of point source ID layer
------------------------------
Compress the point source ID ...

Compression of scan angle layer
------------------------------
Compress the scan angle ...

Compression of user data layer
------------------------------
Compress the user data layer ...

Compression of RGB layer
------------------------------
Compress the RGB layer ...

Compression of NIR layer
------------------------------
Compress the NIR layer ...

Compression of WavePacket layer
------------------------------
Compress the WavePacket layer ...

Compression of "Extra Bytes" layer
------------------------------
Compress the "Extra Bytes" layer ...
