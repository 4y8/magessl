#include <stdint.h>
#include <string.h>

#include "dgst.h"
#include "endian.h"

#define F(B, C, D)	((((C) ^ (D)) & (B)) ^ (D))
#define G(B, C, D)	((((C) ^ (B)) & (D)) ^ (C))
#define H(B, C, D)	((B) ^ (C) ^ (D))
#define I(B, C, D)	((C) ^ ((B) | ~(D)))

static void md5_round(unsigned char *buf, uint32_t out[4]);

static uint32_t K[64] = {
	0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
	0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
	0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
	0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
	0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
	0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
	0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
	0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
	0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
	0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
	0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
	0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
	0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
	0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
	0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
	0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
};

static uint32_t sht[64] = {
	[16] = 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 5, 8, 11,
	14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 0, 7, 14, 5, 12, 3, 10, 1,
	8, 15, 6, 13, 4, 11, 2, 9
};

static void
md5_round(unsigned char *buf, uint32_t out[4])
{
	uint32_t w[16], a, b, c, d;
	int i;

	a = out[0];
	b = out[1];
	c = out[2];
	d = out[3];

	dec_blk32le(buf, 64, w);

	for (i = 0; i < 16; i += 4) {
		a = b + ROTL32(a + F(b, c, d) + w[i]     + K[i],      7);
		d = a + ROTL32(d + F(a, b, c) + w[i + 1] + K[i + 1], 12);
		c = d + ROTL32(c + F(d, a, b) + w[i + 2] + K[i + 2], 17);
		b = c + ROTL32(b + F(c, d, a) + w[i + 3] + K[i + 3], 22);
	}
	for (; i < 32; i += 4) {
		a = b + ROTL32(a + G(b, c, d) + w[sht[i]]     + K[i],      5);
		d = a + ROTL32(d + G(a, b, c) + w[sht[i + 1]] + K[i + 1],  9);
		c = d + ROTL32(c + G(d, a, b) + w[sht[i + 2]] + K[i + 2], 14);
		b = c + ROTL32(b + G(c, d, a) + w[sht[i + 3]] + K[i + 3], 20);
	}
	for (; i < 48; i += 4) {
		a = b + ROTL32(a + H(b, c, d) + w[sht[i]]     +  K[i],     4);
		d = a + ROTL32(d + H(a, b, c) + w[sht[i + 1]] + K[i + 1], 11);
		c = d + ROTL32(c + H(d, a, b) + w[sht[i + 2]] + K[i + 2], 16);
		b = c + ROTL32(b + H(c, d, a) + w[sht[i + 3]] + K[i + 3], 23);
	}
	for (; i < 64; i += 4) {
		a = b + ROTL32(a + I(b, c, d) + w[sht[i]]     + K[i],      6);
		d = a + ROTL32(d + I(a, b, c) + w[sht[i + 1]] + K[i + 1], 10);
		c = d + ROTL32(c + I(d, a, b) + w[sht[i + 2]] + K[i + 2], 15);
		b = c + ROTL32(b + I(c, d, a) + w[sht[i + 3]] + K[i + 3], 21);
	}

	out[0] += a;
	out[1] += b;
	out[2] += c;
	out[3] += d;
}


int
md5_dgst(uint64_t insize, unsigned char *in, unsigned char out[16])
{
	uint64_t i;
	uint32_t h[4];
	unsigned char buf[64];

	h[0] = 0x67452301;
	h[1] = 0xEFCDAB89;
	h[2] = 0x98BADCFE;
	h[3] = 0x10325476;
	for (i = 0; i + 64 < insize; i += 64)
		md5_round(in + i, h);
	memset(buf, 0, 16 * sizeof(uint32_t));
	memcpy(buf, in + i, insize - i);
	buf[insize - i] = 0x80;

	*((uint64_t *)(buf + 56)) = insize << 3;
	md5_round(buf, h);
	enc_blk32le(h, 4, out);
	return 1;
}
