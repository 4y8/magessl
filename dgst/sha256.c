#include <stdint.h>
#include <string.h>

#include "dgst.h"
#include "endian.h"

#define CH(x, y, z)	(((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z)	(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define BSIG0(x)	(ROTR32(x, 2) ^ ROTR32(x, 13) ^ ROTR32(x, 22))
#define BSIG1(x)	(ROTR32(x, 6) ^ ROTR32(x, 11) ^ ROTR32(x, 25))
#define SSIG0(x)	(ROTR32(x, 7) ^ ROTR32(x, 18) ^ ((x) >> 3))
#define SSIG1(x)	(ROTR32(x, 17) ^ ROTR32(x, 19) ^ ((x) >> 10))

static void sha256_round(unsigned char *buf, uint32_t out[8]);

static uint32_t K[64] = {
	0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
	0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
	0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
	0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
	0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
	0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
	0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
	0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
	0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
	0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
	0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
	0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
	0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
	0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
	0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
	0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};

static void
sha256_round(unsigned char *buf, uint32_t out[8])
{
	uint32_t w[64];
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t t1, t2;
	int i;

	a = out[0];
	b = out[1];
	c = out[2];
	d = out[3];
	e = out[4];
	f = out[5];
	g = out[6];
	h = out[7];

	dec_blk32be(buf, 64, w);

	for (i = 16; i < 64; ++i)
		w[i] = SSIG1(w[i - 2]) + w[i - 7] + SSIG0(w[i - 15]) + w[i - 16];

	for (i = 0; i < 64; ++i) {
		t1 = h + BSIG1(e) + CH(e, f, g) + K[i] + w[i];
		t2 = BSIG0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	out[0] += a;
	out[1] += b;
	out[2] += c;
	out[3] += d;
	out[4] += e;
	out[5] += f;
	out[6] += g;
	out[7] += h;
}

int
sha256_dgst(uint64_t insize, unsigned char *in, unsigned char out[32])
{
	unsigned char buf[64];
	uint32_t h[8];
	uint64_t i;

	h[0] = 0x6A09E667;
	h[1] = 0xBB67AE85;
	h[2] = 0x3C6EF372;
	h[3] = 0xA54FF53A;
	h[4] = 0x510E527F;
	h[5] = 0x9B05688C;
	h[6] = 0x1F83D9AB;
	h[7] = 0x5BE0CD19;
	for (i = 0; i + 64 < insize; i += 64)
		sha256_round(in + i, h);
	memset(buf, 0, 16 * sizeof(uint32_t));
	memcpy(buf, in + i, insize - i);
	buf[insize - i] = 0x80;

	enc64be(insize << 3, buf + 56);
	sha256_round(buf, h);
	enc_blk32be(h, 8, out);
	return 1;
}

int
sha224_dgst(uint64_t insize, unsigned char *in, unsigned char out[28])
{
	unsigned char buf[64];
	uint32_t h[8];
	uint64_t i;

	h[0] = 0xC1059ED8;
	h[1] = 0x367CD507;
	h[2] = 0x3070DD17;
	h[3] = 0xF70E5939;
	h[4] = 0xFFC00B31;
	h[5] = 0x68581511;
	h[6] = 0x64F98FA7;
	h[7] = 0xBEFA4FA4;
	for (i = 0; i + 64 < insize; i += 64)
		sha256_round(in + i, h);
	memset(buf, 0, 16 * sizeof(uint32_t));
	memcpy(buf, in + i, insize - i);
	buf[insize - i] = 0x80;

	enc64be(insize << 3, buf + 56);
	sha256_round(buf, h);
	enc_blk32be(h, 7, out);
	return 1;
}
