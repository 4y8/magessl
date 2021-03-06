#include <stdint.h>
#include <string.h>

#include "dgst.h"
#include "endian.h"

#define F(B, C, D)	((((C) ^ (D)) & (B)) ^ (D))
#define G(B, C, D)	((B) ^ (C) ^ (D))
#define H(B, C, D)	(((D) & (C)) | (((D) | (C)) & (B)))
#define I(B, C, D)	((B) ^ (C) ^ (D))

static void sha1_round(unsigned char *buf, uint32_t out[5]);

static uint32_t IV[4] = {
	0x5A827999,
	0x6ED9EBA1,
	0x8F1BBCDC,
	0xCA62C1D6
};

static void
sha1_round(unsigned char *buf, uint32_t out[5])
{
	uint32_t w[80], a, b, c, d, e, t;
	int i;

	a = out[0];
	b = out[1];
	c = out[2];
	d = out[3];
	e = out[4];

	dec_blk32be(buf, 64, w);

	for (i = 16; i < 80; ++i)
		w[i] = ROTL32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i -16], 1);

	for (i = 0; i < 20; ++i) {
		t = ROTL32(a, 5) + F(b, c, d) + e + w[i] + IV[0];
		e = d;
		d = c;
		c = ROTL32(b, 30);
		b = a;
		a = t;
	}
	for (; i < 40; ++i) {
		t = ROTL32(a, 5) + G(b, c, d) + e + w[i] + IV[1];
		e = d;
		d = c;
		c = ROTL32(b, 30);
		b = a;
		a = t;
	}
	for (; i < 60; ++i) {
		t = ROTL32(a, 5) + H(b, c, d) + e + w[i] + IV[2];
		e = d;
		d = c;
		c = ROTL32(b, 30);
		b = a;
		a = t;
	}
	for (; i < 80; ++i) {
		t = ROTL32(a, 5) + I(b, c, d) + e + w[i] + IV[3];
		e = d;
		d = c;
		c = ROTL32(b, 30);
		b = a;
		a = t;
	}
	out[0] += a;
	out[1] += b;
	out[2] += c;
	out[3] += d;
	out[4] += e;
}


int
sha1_dgst(uint64_t insize, unsigned char *in, unsigned char out[20])
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
		sha1_round(in + i, h);
	memset(buf, 0, 16 * sizeof(uint32_t));
	memcpy(buf, in + i, insize - i);
	buf[insize - i] = 0x80;

	enc64be(insize << 3, buf + 56);
	sha1_round(buf, h);
	enc_blk32be(h, 5, out);
	return 1;
}
