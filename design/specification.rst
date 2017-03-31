LASzip for LAS 1.4 (native extension)
========
Specification Document 
--------
As before the LASheader and the VLRs are stored uncompressed but the highest bit of the point type field is set (i.e. the compressed point types 128 to 138 correspond to the uncompressed point types 0 to 10). LiDAR points are compressed in completely independent chunks that default to 50,000 points per chunk. Each chunk can be decompressed "on its own" once the start byte is know. For point types 0 to 5 nothing changes and the same compression scheme as before is used. For the new point types 6 to 10 of LAS 1.4 there are two main changes: the chunks are encoded in layers and the chunks can have variable size.

As before the first point of each chunk is stored raw. The attributes of all following points are broken into several layers. Only the first layer containing the x and y coordinates (and a few pieces of information) is mandatory to read and decompress. The remaining layers containing independently useful attributes such as elevation, intensity, classifications, flags, GPS times, colors, point source ID, user data, scan angles, wavepackets, and extra bytes do not necessarily need be decompressed (or be read from disk).

Another new (or rather "revived") feature in LASzip compression for the new point types of LAS 1.4 is that it will be possible to continously vary the chunk size. This was in the design of the original LASzip coder but broke whenever the file was streamed due to missing access to the per chunk point counts during a streaming read as those were only stored in the chunk table at the very end of the LAZ file. This is fixed now (but only for the new point types) by also storing the number of points at the beginning of each chunk.

LAZ File Layout:
----------------

1. LASheader
2. VLRs + LASzip VLR
3. Sequence of n Chunks

   1. Chunk 1
   2. Chunk 2
   3. Chunk 3
   ....
   n. Chunk n

4. Chunk Table (chunk starts and point numbers)
5. EVLRs
6. LASindex EVLR (future feature: optional index for faster spatial queries)
7. LASlayers EVLR (future feature: for in-place edits of classifications / flags)

Chunk Layout:
-------------
1) Raw point [30 - 68 (or more) bytes]
2) Numbers and Bytes
  + Number of *remaining* points [4 bytes]
  + Number of bytes (maybe compressed)
     - scanner channel, point source ID change, GPS time change, scan angle change, return counts, and XY layer [4 bytes]
     - Z layer [4 bytes]
     - classification layer [4 byte]
     - flags layer [4 bytes]
     - intensity layer [4 bytes]
     - scan angle layer [4 bytes]
     - user data layer [4 bytes]
     - point source ID layer [4bytes]
     - GPS time layer [4 bytes]
     optional
     - RGB layer [4 bytes]
     - RGBNIR layer [4 bytes]
     - WavePacket layer [4 bytes]
     - "Extra Bytes" layer [4 bytes per byte]
3) Layers
     - scanner channel, point source ID change, GPS time change, scan angle change, return counts, and XY layer
     - Z layer
     - classification layer
     - flags layer
     - intensity layer
     - scan angle layer
     - user data layer
     - point source ID layer
     - GPS time layer
     optional
     - RGB layer
     - RGBNIR layer
     - WavePacket layer
     - "Extra Bytes" layer

Compression of scanner channel, point source ID change, GPS time change, scan angle change, return counts, and XY layer
-----------------------------------------------------------------------------
Due to the new scanner channel it is *crucial* to first encode whether a point is from the same and if not from which other scanner channel so that the correct context can be used for all subsequent predictions. We also encode whether a point has a a different point source ID, a different GPS time, and a different scan angle as the previous point from the same scanner channel as these changes correlate strongly with another. The return counts that are also recorded in this layer.

* scanner channel compared to the scanner channel of the previous point (same = 0 / different = 1)
* point source ID compared to the point source ID of the previous point from the *same* scanner channel (same = 0 / different = 1)
* GPS time stamp compared to the GPS time stamp of the previous point from the *same* scanner channel (same = 0 / different = 1)
* scan angle compared to the scan angle of the previous point from the *same* scanner channel (same = 0 / different = 1)
* number of returns compared to the number of returns of the previous point from the *same* scanner channel (same = 0 / different = 1)
* return number compared to the return number of th previous point from the *same* scanner channel (same = 0 / plus one = 1 / minus one = 2 / other difference = 3)

These 7 bits of information are combined into one symbol whose value ranges from 0 to 127 that we then compress with one of four (4) different contexts based on whether the *directly* previous point (no matter from which scanner channel) was a single return (0), or the first (1), the last (2) or the intermediate (3) return in case of multi-return.

If the **scanner channel is different** we use one symbol whose value ranges from 0 to 2 to encode whether we need to add 1, 2, or 3 to the previous scanner channel to get (modulo 4) to the current scanner channel that we then compress using the previous scanner channel as one of four different contexts. All following predictions are relative to the previous point from the *same* scanner channel. If that point does not yet exist then the previous point (from whichever other scanner channel) is used. For the very first point per chunk *that gets compressed* this is that one point that is stored raw at the beginning of every chunk.

If the **number of returns is different** we use one symbol whose value ranges from 0 to 15 that we then compress with the previous number of returns (from the same scanner channel) as one of sixteen contexts.

If the **return number is different** we encode in in two possible ways depending on whether the GPS time stamp has changed:
   - if the GPS time stamp *has not* changed we use one symbol whose value ranges from 0 to 12 to encode whether we need to add 2, 3, 4 ... 12, 13 or 14 to the previous return number (from the same scanner channel) to get (modulo 16) to the current return number that we then compress using the previous return number (from the same scanner channel) as one of sixteen contexts.
   - if the GPS time stamp *has* changed we use one symbol whose value ranges from 0 to 15 to encode the current return number that we then compress with the (already encoded) number of returns as one of sixteen contexts.

Next we use the number of returns n and the return number r to to derive two numbers between 0 and 15 that are used for switching contexts later: a *return map m* and *a return level l*.

The *return map m* simply serializes the valid combinations of r and n: for r = 1 and n = 1 it is 0, for r = 1 and n = 2 it is 1, for r = 2 and n = 2 it is 2, for r = 1 and n = 3 it is 3, for r = 2 and n = 3 it is 4, for r = 3 and n = 3 it is 5, for r = 1 and n = 4 it is 6, etc. Unfortunately, some LAS files start numbering r and n at 0, only have return numbers r, or only have number of return of given pulse counts n. We therefore complete the table to also map invalid combinations to be used as a context. Furthermore with up to 15 returns there would be too many different combinations (namely 120). Therefore we map combinations of many returns pulses to the same number m and also complete the table for all combinations of r and n as shown below:

const U8 number_return_map_4bit[16][16] = 
{
  { 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0 },
  { 14,  0,  1,  3,  6, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
  { 13,  1,  2,  4,  7, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10 },
  { 12,  3,  4,  5,  8, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 11 },
  { 11,  6,  7,  8,  9, 13, 13, 12, 12, 12, 12, 11, 11, 11, 11, 11 },
  { 10, 10, 11, 12, 13, 14, 14, 13, 13, 12, 12, 12, 12, 12, 12, 12 },
  {  9, 10, 11, 12, 13, 14, 15, 14, 13, 13, 13, 12, 12, 12, 12, 12 },
  {  8, 10, 11, 12, 12, 13, 14, 15, 14, 13, 13, 13, 13, 12, 12, 12 },
  {  7, 10, 11, 12, 12, 13, 13, 14, 15, 14, 14, 13, 13, 13, 13, 13 },
  {  6, 10, 11, 11, 12, 12, 13, 13, 14, 15, 14, 14, 14, 13, 13, 13 },
  {  5, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 14, 14, 14, 13, 13 },
  {  4, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 14, 14, 14 },
  {  3, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 15, 14, 14 },
  {  2, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 15, 14 },
  {  1, 10, 10, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 15, 15, 15 },
  {  0, 10, 10, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15 }
};

The *return level l* specifies how many returns there have already been for a given pulse prior to this return. Given only valid combinations for the return number r and the number of returns of given pulse n we could compute it as l = n − r. But we again use a completed look-up table as shown below to map invalid combinations for r and l to different contexts.

const U8 number_return_level_4bit[16][16] = 
{
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 12, 14, 15 },
  {  1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 12, 14 },
  {  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 12 },
  {  3,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12 },
  {  4,  3,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11 },
  {  5,  4,  3,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 },
  {  6,  5,  4,  3,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9 },
  {  7,  6,  5,  4,  3,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,  8 },
  {  8,  7,  6,  5,  4,  3,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7 },
  {  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  1,  2,  3,  4,  5,  6 },
  { 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  1,  2,  3,  4,  5 },
  { 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  1,  2,  3,  4 },
  { 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  1,  2,  3 },
  { 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  1,  2 },
  { 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,  1 },
  { 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0 }
};

We encode the X and Y coordinates with a second order predictor. We predict the differences dX and dY to the X and Y coordinates of the previous point from the same scanner channel with recently occuring differences. We predicts dX and dY between as the median of the five immediately preceding differences of points from the scanner channel and with the same return map m. The intuition behind this is, for example, that single returns are always from a different laser pulse than the previous point and therefore have a wider spacing in X and/or Y than the middle of three returns.

Compression of classification layer
-----------------------------------
Compress the classification based on the context m and the previous value similar to the existing LASzip.

Compression of flags layer
--------------------------

Compression of intensity layer
------------------------------
Compress the intensity based on the context m similar to the existing LASzip.

Compression of scan angle layer
------------------------------
Compress the scan angle ...

Compression of user data layer
------------------------------
Compress the user data layer ...

Compression of point source ID layer
------------------------------
Compress the user data layer ...

Compression of GPS time layer
-----------------------------
The GPS times of a single flight line stored in aquisition order are a monotonically increasing sequence of double-precision floating-point numbers where returns of the same pulse have the same GPS time and where subsequent pulses have a more or less constant spacing in time. LASzip treats the double-precision floating-point GPS times as signed 64 bit integers and predicts the deltas between them. We store up to four previously compressed GPS times with corresponding deltas to account for repeated jumps in GPS times that can arise when multiple flight paths are merged with fine spatial granularity. Nothing needs to be encoded when the GPS times are identical. Otherwise we distinguishes several cases that are entropy coded with 515 symbols depending on if the current GPS time is

   +   0–500 : predicted using the current delta times 0 to 500.
   + 501–510 : predicted using the current delta times -1 to -10.
   +     511 : starting a new context.
   + 512–514 : predicted with one of the other three contexts.
   
For the first two cases we subsequently difference code the delta prediction and the actual delta. We starts a new context when the delta overflows a 32 bit integer. For that it difference codes the 32 higher bits of the current GPS time and the current context and stores the lower 32 bits raw. Otherwise it switches to the specified context (where the delta will not overflows a 32 bit integer) and recurses. The current deltas stored with each context are updated to the actual delta when they were outside the predictable range more than 3 consecutive times (i.e. bigger than 500 times the current delta, smaller than -10 times the current delta, or zero).

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
