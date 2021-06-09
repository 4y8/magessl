#include <string.h>

#include <dgst.h>
#include <endian.h>

enum {
	R1 = 32,
	R2 = 24,
	R3 = 16,
	R4 = 63
};

ulong IV[8] = {
	0X6A09E667F3BCC908,
	0XBB67AE8584CAA73B,
	0X3C6EF372FE94F82B,
	0XA54FF53A5F1D36F1,
	0X510E527FADE682D1,
	0X9B05688C2B3E6C1F,
	0X1F83D9ABFB41BD6B,
	0X5BE0CD19137E2179
};

uint sigma[12][16] = {
	{ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
	{14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3},
	{11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4},
	{ 7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8},
	{ 9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13},
	{ 2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9},
	{12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11},
	{13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10},
	{ 6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5},
	{10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0},
	{ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
	{14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3}
};

static void mix(ulong v[16], int a, int b, int c, int d, ulong x, ulong y);
static void compress(ulong h[8], ulong m[16], ulong t[2], int islast);

static void
mix(ulong v[16], int a, int b, int c, int d, ulong x, ulong y)
{
	v[a] = v[a] + v[b] + x;
	v[d] = ROTR64(v[d] ^ v[a], R1);
	v[c] = v[c] + v[d];
	v[b] = ROTR64(v[b] ^ v[c], R2);
	v[a] = v[a] + v[b] + y;
	v[d] = ROTR64(v[d] ^ v[a], R3);
	v[c] = v[c] + v[d];
	v[b] = ROTR64(v[b] ^ v[c], R4);
}

static void
compress(ulong h[8], ulong m[16], ulong t[2], int islast)
{
	ulong v[16];

	memcpy(v, h, 8 * sizeof(ulong));
	memcpy(v + 8, IV, 8 * sizeof(ulong));

	v[12] ^= t[0];
	v[13] ^= t[1];

	if (islast)
		v[14] = ~v[14];

	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 4; ++j)
			mix(v, i, i + 4, i + 8, i + 12, m[sigma[i][j * 2]],
			    m[sigma[i][j * 2 + 1]]);

		mix(v, 0, 5, 10, 15, m[sigma[i][ 8]], m[sigma[i][ 9]]);
		mix(v, 1, 6, 11, 12, m[sigma[i][10]], m[sigma[i][11]]);
		mix(v, 2, 7,  8, 13, m[sigma[i][12]], m[sigma[i][13]]);
		mix(v, 3, 4,  9, 14, m[sigma[i][14]], m[sigma[i][15]]);

	}

	for (int i = 0; i < 8; ++i)
		h[i] ^= v[i] ^ v[i + 8];
}

int
blake2b_512_dgst(ulong insize, uchar *in, ulong keysize,
                 uchar *key, uchar out[64])
{
	ulong h[8];

	return 0;
}
