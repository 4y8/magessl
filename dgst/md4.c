#include <stdint.h>
#include <string.h>

#include "dgst.h"
#include "endian.h"

#define F(B, C, D)	((((C) ^ (D)) & (B)) ^ (D))
#define G(B, C, D)	(((B) & (C)) | ((B) & (D)) | ((C) & (D)))
#define H(B, C, D)	((B) ^ (C) ^ (D))

#define K2 0x5a827999
#define K3 0x6ed9eba1

static void md4_round(unsigned char *buf, uint32_t out[4]);

static void
md4_round(unsigned char *buf, uint32_t out[4])
{
	uint32_t w[16], a, b, c, d;
	int i, m;

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
	for (; i < 32; i += 4) {
		a = ROTL32(a + G(b, c, d) + w[i & 0b11]       + K2,  3);
		d = ROTL32(d + G(a, b, c) + w[(i & 0b11) + 1] + K2,  5);
		c = ROTL32(c + G(d, a, b) + w[(i & 0b11) + 2] + K2,  9);
		b = ROTL32(b + G(c, d, a) + w[(i & 0b11) + 3] + K2, 13);
	}
	for (; i < 48; i += 4) {
		a = ROTL32(a + H(b, c, d) + w[(i << 3) & 0xF]       + K3,  3);
		d = ROTL32(d + H(a, b, c) + w[((i + 1) << 3) & 0xF] + K3,  9);
		c = ROTL32(c + H(d, a, b) + w[((i + 2) << 3) & 0xF] + K3, 11);
		b = ROTL32(b + H(c, d, a) + w[((i + 3) << 3) & 0xF] + K3, 15);
	}

	out[0] += a;
	out[1] += b;
	out[2] += c;
	out[3] += d;
}
