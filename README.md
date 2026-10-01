# Rotated Bits

A C routine that rotates a 1-bit-deep Macintosh `BitMap` 90° clockwise, written in 1993 by Jeff Mallett.

```text
source           rotated 90° clockwise
#####            ###
#....            ..#
#....            ..#
                 ..#
                 ..#
```

It was an entry in the [MacTech Magazine](https://en.wikipedia.org/wiki/MacTech) Programmers' Challenge for April 1993, which asked for a fast `RotateBitMapClockwise()` that writes into a destination `BitMap` whose memory, `rowBytes` and bounds are already set up. Entries were judged on correctness, speed, size and elegance, in that order. Of 16 entries, two were disqualified for using assembly and six for giving wrong results. Steve Israelson won; this entry had the smallest code of the eight correct ones and was third fastest on small bitmaps, and the June 1993 issue published it after the winner for being so small:

| Entry | Code bytes | Test 1 (ticks) | Test 2 (ticks) |
|---|---:|---:|---:|
| Steve Israelson | 740 | 72 | 638 |
| Andy Scheck | 386 | 75 | 778 |
| **Jeff Mallett** | **194** | **91** | **936** |
| Patrick Breen | 668 | 125 | 902 |
| Stepan Riha | 656 | 122 | 1186 |
| Dave Darrah | 362 | 138 | 1453 |
| Jan Bruyndonckx | 490 | 205 | 1785 |
| Dominic Mazzoni | 244 | 630 | 6687 |

Test 1 rotated 4,000 small bitmaps (20×30 and smaller); test 2 rotated 400 bitmaps of about 300×400.

The routine walks the source one column at a time. For each column it builds the destination row 16 bits at a time: the `COPY_TWO_BYTES` macro tests one bit in each of 16 successive source rows and packs the results into a 16-bit word, then a final partial word handles row counts that aren't a multiple of 16.

The code is [`rotated-bits.c`](rotated-bits.c). [`mactech/`](mactech/) has links to the published [challenge](http://preserve.mactech.com/articles/mactech/Vol.09/09.04/Apr93Challenge/index.html) (April 1993) and [results](http://preserve.mactech.com/articles/mactech/Vol.09/09.06/Jun93Challenge/index.html) (June 1993), saved as PDFs, and a scan of the magazine pages.

This is historical source for the classic Mac OS: it uses the Toolbox `BitMap` type and is not a standalone program. MacTech's online copy of the article lost the whitespace in the two `#define` lines (`kHighShortBit0x8000`); it is restored here.

[`test-rotate.c`](test-rotate.c) checks this on a modern compiler (`cc -o test-rotate test-rotate.c && ./test-rotate`). It prints a 32×32 letter F before and after rotation next to the expected result, then checks random bitmaps of several sizes against a true clockwise rotation, including sizes like 9×17 that aren't a multiple of 16.
