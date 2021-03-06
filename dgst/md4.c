#include <stdint.h>
#include <string.h>

#include <dgst.h>
#include <endian.h>

#define F(B, C, D)	((((C) ^ (D)) & (B)) ^ (D))
#define G(B, C, D)	(((B) & (C)) | ((B) & (D)) | ((C) & (D)))
#define H(B, C, D)	((B) ^ (C) ^ (D))

static void md4_round(uchar *buf, uint32_t out[4]);

#define K2 0x5A827999
#define K3 0x6ED9EBA1

static uint32_t sht[16] = {
	0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15
};

static void
md4_round(uchar *buf, uint32_t out[4])
{
	uint32_t w[16], a, b, c, d;
	int i;

	a = out[0];
	b = out[1];
	c = out[2];
	d = out[3];

	dec_blk32le(buf, 64, w);

	for (i = 0; i < 16; i += 4) {
		a = ROTL32(a + F(b, c, d) + w[i],      3);
		d = ROTL32(d + F(a, b, c) + w[i + 1],  7);
		c = ROTL32(c + F(d, a, b) + w[i + 2], 11);
		b = ROTL32(b + F(c, d, a) + w[i + 3], 19);
	}
	for (i = 0; i < 16; i += 4) {
		a = ROTL32(a + G(b, c, d) + w[i >> 2]       + K2,  3);
		d = ROTL32(d + G(a, b, c) + w[(i >> 2) + 4] + K2,  5);
		c = ROTL32(c + G(d, a, b) + w[(i >> 2) + 8] + K2,  9);
		b = ROTL32(b + G(c, d, a) + w[(i >> 2) + 12] + K2, 13);
	}
	for (i = 0; i < 16; i += 4) {
		a = ROTL32(a + H(b, c, d) + w[sht[i]]     + K3,  3);
		d = ROTL32(d + H(a, b, c) + w[sht[i + 1]] + K3,  9);
		c = ROTL32(c + H(d, a, b) + w[sht[i + 2]] + K3, 11);
		b = ROTL32(b + H(c, d, a) + w[sht[i + 3]] + K3, 15);
	}

	out[0] += a;
	out[1] += b;
	out[2] += c;
	out[3] += d;
}

int
md4_dgst(ulong insize, uchar *in, uchar out[16])
{
	uint64_t i;
	uint32_t h[4];
	uchar buf[64];

	h[0] = 0x67452301;
	h[1] = 0xEFCDAB89;
	h[2] = 0x98BADCFE;
	h[3] = 0x10325476;
	for (i = 0; i + 64 < insize; i += 64)
		md4_round(in + i, h);
	memset(buf, 0, 64);
	memcpy(buf, in + i, insize - i);
	buf[insize - i] = 0x80;

	*((uint64_t *)(buf + 56)) = insize << 3;
	md4_round(buf, h);
	enc_blk32le(h, 4, out);
	return 1;
}
