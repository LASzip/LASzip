As before the LiDAR points are compressed in completely independent chunks that default to 50,000 points per chunk. Each chunk can be decompressed "on its own" once the start byte is know. As before the first point of each chunk is stored raw. The attributes of all following points are broken into several layers. Only the first layer containing the x and y coordinates (and a few pieces of information) is mandatory to be read  mandartory. The remaining layers containing independently useful attributes such as elevation, intensity, GPS times, colors, point source ID, classifications and flags do not necessarily need to be read from disk and be decompressed.

Another new (or rather "revived") feature in LASzip compression for the new point types of LAS 1.4 is that it will be possible to continously vary the chunk size. This in the design of the original LASzip but broke when the file was streamed due to missing access to the per chunk point counts during a streaming read as those were only stored in the chunk table at the end of the LAZ file. This will be fixed by also storing the number of points inside each chunk.

LAZ File Layout:
----------------

1. LASheader
2. LASzip VLR
3. Sequence of n Chunks

   1. Chunk 1
   2. Chunk 2
   3. Chunk 3
   ....
   n. Chunk n

4. Chunk Table (chunk starts and point numbers)
5. EVLRs
6. LASindex EVLR (optional index)
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
* number of returns different from previous point of same scanner channel (yes / no)
* return number different from previous point of same scanner channel (yes / no)

These 4 bits are combined into one symbol whose value ranges from 0 to 15 that we then compress with one of six (6) different contexts based on whether the directly previous point was a single return (0), the first (1) or the last (2) of a double return, or the first (3), an intermediate (4), or the last (5) of a triple or higher return.

If the scanner channel is different we use one symbol whose value ranges from 0 to 2 to encode whether we need to add 1, 2, or 3 to the previous scanner channel to get (modulo 4) to the current scanner channel that we then compress using the previous scanner channel as one of four different contexts. The following XYZ coordinates and attribute predictions are relative to the previous point from the *same* scanner channel. For each chunk the points of all four channels are initialized to the first point per chunks that is stored raw. 

If the number of returns is different we use one symbol whose value ranges from 0 to 15 that we then compress it with the previous number of returns (of same scanner channel) as one of sixteen contexts.

If the return number is different we encode in in two possible ways depending on whether the GPS time stamp has changed:
   - if the GPS time stamp *has* *not* changed we use one symbol whose value ranges from 0 to 14 to encode whether we need to add 1, 2, 3, 4 ... , 14 or 15 to the previous return number (of same scanner channel) to get (modulo 16) to the current return number that we then compress using the previous return number (of same scanner channel) as one of sixteen contexts.
   - if the GPS time stamp *has* changed we use one symbol whose value ranges from 0 to 15 to encode the current return number that we then compress with a single context.

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
