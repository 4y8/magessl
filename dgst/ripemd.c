#include <stdint.h>
#include <string.h>

#include "dgst.h"
#include "endian.h"

#define F(B, C, D)	((B) ^ (C) ^ (D))
#define G(B, C, D)	((((C) ^ (D)) & (B)) ^ (D))
#define H(B, C, D)	((~(C) | (B)) ^ (D))
#define I(B, C, D)	((((B) ^ (C)) & (D)) ^ (C))
#define J(B, C, D)	((~(D) | (C)) ^ (B))

#define KL0	0x00000000
#define KL1	0x5A827999
#define KL2	0x6ED9EBA1
#define KL3	0x8F1BBCDC
#define KL4	0xA953FD4E

#define KR0	0x50A28BE6
#define KR1	0x5C4DD124
#define KR2	0x6D703EF3
#define KR3	0x7A6D76E9
#define KR4	0x00000000

static void ripemd_round(unsigned char *buf, uint32_t out[5]);


static uint32_t shtl[80] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
	3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
	1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
	4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13
};

static uint32_t shtr[80] = {
	5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
	6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
	15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
	8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
	12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11
};

static uint32_t rotl[80] = {
	11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
	7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
	11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
	11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
	9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6
};

static uint32_t rotr[80] = {
	8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
	9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
	9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
	15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
	8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11
};

static void
ripemd_round(unsigned char *buf, uint32_t out[5])
{
	uint32_t w[16];
	uint32_t al, bl, cl, dl, el;
	uint32_t ar, br, cr, dr, er;
	uint32_t t;
	int i;

	al = out[0];
	bl = out[1];
	cl = out[2];
	dl = out[3];
	el = out[4];

	ar = out[0];
	br = out[1];
	cr = out[2];
	dr = out[3];
	er = out[4];

	dec_blk32le(buf, 64, w);

	for (i = 0; i < 16; ++i) {
		t = ROTL32(al + F(bl, cl, dl) + w[shtl[i]] + KL0, rotl[i]) + el;
		al = el;
		el = dl;
		dl = ROTL32(cl, 10);
		cl = bl;
		bl = t;

		t = ROTL32(ar + J(br, cr, dr) + w[shtr[i]] + KR0, rotr[i]) + er;
		ar = er;
		er = dr;
		dr = ROTL32(cr, 10);
		cr = br;
		br = t;
	}
	for (; i < 32; ++i) {
		t = ROTL32(al + G(bl, cl, dl) + w[shtl[i]] + KL1, rotl[i]) + el;
		al = el;
		el = dl;
		dl = ROTL32(cl, 10);
		cl = bl;
		bl = t;

		t = ROTL32(ar + I(br, cr, dr) + w[shtr[i]] + KR1, rotr[i]) + er;
		ar = er;
		er = dr;
		dr = ROTL32(cr, 10);
		cr = br;
		br = t;
	}
	for (; i < 48; ++i) {
		t = ROTL32(al + H(bl, cl, dl) + w[shtl[i]] + KL2, rotl[i]) + el;
		al = el;
		el = dl;
		dl = ROTL32(cl, 10);
		cl = bl;
		bl = t;

		t = ROTL32(ar + H(br, cr, dr) + w[shtr[i]] + KR2, rotr[i]) + er;
		ar = er;
		er = dr;
		dr = ROTL32(cr, 10);
		cr = br;
		br = t;
	}
	for (; i < 64; ++i) {
		t = ROTL32(al + I(bl, cl, dl) + w[shtl[i]] + KL3, rotl[i]) + el;
		al = el;
		el = dl;
		dl = ROTL32(cl, 10);
		cl = bl;
		bl = t;

		t = ROTL32(ar + G(br, cr, dr) + w[shtr[i]] + KR3, rotr[i]) + er;
		ar = er;
		er = dr;
		dr = ROTL32(cr, 10);
		cr = br;
		br = t;
	}
	for (; i < 80; ++i) {
		t = ROTL32(al + J(bl, cl, dl) + w[shtl[i]] + KL4, rotl[i]) + el;
		al = el;
		el = dl;
		dl = ROTL32(cl, 10);
		cl = bl;
		bl = t;

		t = ROTL32(ar + F(br, cr, dr) + w[shtr[i]] + KR4, rotr[i]) + er;
		ar = er;
		er = dr;
		dr = ROTL32(cr, 10);
		cr = br;
		br = t;
	}
	t = out[1] + cl + dr;
	out[1] = out[2] + dl + er;
	out[2] = out[3] + el + ar;
	out[3] = out[4] + al + br;
	out[4] = out[0] + bl + cr;
	out[0] = t;
}

int
ripemd_dgst(uint64_t insize, unsigned char *in, unsigned char out[20])
{
	unsigned char buf[64];
	uint32_t h[5];
	uint64_t i;

	h[0] = 0x67452301;
	h[1] = 0xEFCDAB89;
	h[2] = 0x98BADCFE;
	h[3] = 0x10325476;
	h[4] = 0xC3D2E1F0;
	for (i = 0; i + 64 < insize; i += 64)
		ripemd_round(in + i, h);
	memset(buf, 0, 16 * sizeof(uint32_t));
	memcpy(buf, in + i, insize - i);
	buf[insize - i] = 0x80;

	*((uint64_t *)(buf + 56)) = insize << 3;
	ripemd_round(buf, h);
	enc_blk32le(h, 5, out);
	return 1;
}
