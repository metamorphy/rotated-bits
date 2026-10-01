//*********************************************************
// RotateBitMapClockwise
// By Jeff Mallett
//
// Rotates the significant bits of the srcBitMapPtr 90°
// clockwise and stores the result in dstBitMapPtr.
//*********************************************************

#define kHighShortBit	0x8000
#define kBitsPerShort	16

// Creates a short from bits in different rows of
// the source bitmap.  Then copies this short into
// the destination bitmap.
#define COPY_TWO_BYTES(stopValue)  \
	data = 0, bit = kHighShortBit;	\
	do {   \
		if (*srcPos & srcBit) data |= bit;\
		srcPos -= srcRowShorts;  \
	} while ( (bit >>= 1) != stopValue ); \
	*(dstPos++) = data

void RotateBitMapClockwise(BitMap *srcBitMapPtr, BitMap *dstBitMapPtr)
{
	register unsigned short data, *srcPos, bit;
	register int j;
	register unsigned short srcBit, *dstPos, stopBit;
	register int i;
	unsigned short *baseSrcPtr;
	// shorts per row of source
	const int srcRowShorts = srcBitMapPtr->rowBytes >> 1;
	const int numSrcRows = srcBitMapPtr->bounds.bottom - srcBitMapPtr->bounds.top;

	dstPos = (unsigned short *)dstBitMapPtr->baseAddr;
	// Start at the bottom row; each column is read bottom to top
	srcPos = baseSrcPtr = (unsigned short *)srcBitMapPtr->baseAddr
							+ (numSrcRows - 1) * srcRowShorts;
	srcBit = kHighShortBit;
 
	for (i = srcBitMapPtr->bounds.right - srcBitMapPtr->bounds.left; i; --i) {
		// Copy one column of source to a row of destination
		for (j = numSrcRows / kBitsPerShort; j; --j) {
			COPY_TWO_BYTES(0);
		}
		if (j = numSrcRows % kBitsPerShort) {
			stopBit = kHighShortBit >> j;
			COPY_TWO_BYTES(stopBit); // Final 2 bytes
		}
		// Prepare to copy next column of source
		if ( !(srcBit >>= 1) ) {
			srcBit = kHighShortBit;
			++baseSrcPtr;
		}
		srcPos = baseSrcPtr;
	}
}