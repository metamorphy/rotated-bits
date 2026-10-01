//*********************************************************
// test-rotate.c
// By Claude in the modern age.
//
// Test for RotateBitMapClockwise on a modern compiler.
//   cc -o test-rotate test-rotate.c && ./test-rotate
//
// Prints a 32x32 letter F before and after rotation, next
// to the expected clockwise rotation, then checks random
// bitmaps of several sizes (including ones that aren't a
// multiple of 16). Exits with status 1 if any check fails.
//*********************************************************

#include <stdio.h>
#include <stdlib.h>

// Just enough of the Mac Toolbox for rotated-bits.c
typedef struct { short top, left, bottom, right; } Rect;
typedef struct { char *baseAddr; short rowBytes; Rect bounds; } BitMap;

#include "rotated-bits.c"

// Pixels are stored as on a 68000 Mac: each 16-bit word
// holds 16 pixels, with the leftmost pixel in the high bit.
static int GetPixel(BitMap *b, int row, int col)
{
	unsigned short *w = (unsigned short *)(b->baseAddr + row * b->rowBytes) + col / 16;
	return (*w >> (15 - col % 16)) & 1;
}

static void SetPixel(BitMap *b, int row, int col)
{
	unsigned short *w = (unsigned short *)(b->baseAddr + row * b->rowBytes) + col / 16;
	*w |= kHighShortBit >> (col % 16);
}

// Allocates a cleared bitmap with rowBytes rounded up to
// a whole number of 16-bit words
static BitMap NewBitMap(int rows, int cols)
{
	BitMap b;

	b.rowBytes = ((cols + 15) / 16) * 2;
	b.bounds.top = b.bounds.left = 0;
	b.bounds.bottom = rows;
	b.bounds.right = cols;
	b.baseAddr = calloc(rows, b.rowBytes);
	return b;
}

// Returns true if dst is src rotated 90 degrees clockwise
static int IsClockwise(BitMap *src, BitMap *dst)
{
	int rows = src->bounds.bottom, cols = src->bounds.right;
	int r, c;

	for (r = 0; r < cols; ++r)
		for (c = 0; c < rows; ++c)
			if (GetPixel(dst, r, c) != GetPixel(src, rows - 1 - c, r))
				return 0;
	return 1;
}

static void PrintBitMap(const char *title, BitMap *b)
{
	int r, c;

	printf("%s\n", title);
	for (r = 0; r < b->bounds.bottom; ++r) {
		for (c = 0; c < b->bounds.right; ++c)
			putchar('0' + GetPixel(b, r, c));
		putchar('\n');
	}
	putchar('\n');
}

int main(void)
{
	static const int sizes[][2] = { // rows, columns
		{ 3, 5 }, { 9, 17 }, { 17, 9 }, { 30, 20 },
		{ 1, 1 }, { 16, 16 }, { 33, 47 }, { 400, 300 }
	};
	BitMap src, dst, expected;
	int failures = 0;
	int i, r, c;

	// A letter F: vertical bar on the left, long top arm,
	// shorter middle arm. It looks different in every
	// orientation, so a wrong rotation is easy to spot.
	src = NewBitMap(32, 32);
	dst = NewBitMap(32, 32);
	expected = NewBitMap(32, 32);
	for (r = 4; r < 28; ++r)
		for (c = 6; c < 10; ++c) SetPixel(&src, r, c);
	for (r = 4; r < 8; ++r)
		for (c = 6; c < 26; ++c) SetPixel(&src, r, c);
	for (r = 14; r < 18; ++r)
		for (c = 6; c < 20; ++c) SetPixel(&src, r, c);
	for (r = 0; r < 32; ++r)
		for (c = 0; c < 32; ++c)
			if (GetPixel(&src, 31 - c, r)) SetPixel(&expected, r, c);

	RotateBitMapClockwise(&src, &dst);
	PrintBitMap("BEFORE (input to RotateBitMapClockwise):", &src);
	PrintBitMap("AFTER (what the function produced):", &dst);
	PrintBitMap("EXPECTED (the input rotated 90 degrees clockwise):", &expected);
	if (!IsClockwise(&src, &dst)) {
		printf("FAIL: letter F\n");
		++failures;
	}

	// Random bitmaps of several sizes
	srand(1);
	for (i = 0; i < (int)(sizeof sizes / sizeof sizes[0]); ++i) {
		int rows = sizes[i][0], cols = sizes[i][1];

		src = NewBitMap(rows, cols);
		dst = NewBitMap(cols, rows);
		for (r = 0; r < rows; ++r)
			for (c = 0; c < cols; ++c)
				if (rand() & 1) SetPixel(&src, r, c);
		RotateBitMapClockwise(&src, &dst);
		if (IsClockwise(&src, &dst))
			printf("ok   %dx%d\n", rows, cols);
		else {
			printf("FAIL %dx%d\n", rows, cols);
			++failures;
		}
	}

	printf(failures ? "\n%d check(s) failed\n" : "\nAll checks passed\n", failures);
	return failures != 0;
}
