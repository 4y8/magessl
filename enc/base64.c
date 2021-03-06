#include <stdint.h>

#include "enc.h"
#include "endian.h"

static uchar int2base64(uchar c);
static uchar base642int(uchar c);

static uchar
int2base64(uchar c)
{
	if (c <= 25)
		return c + 'A';
	if (c <= 51)
		return c - 26 + 'a';
	if (c <= 61)
		return c - 52 + '0';
	return c == 62 ? '+' : '/';
}

static uchar
base642int(uchar c)
{
	if ('A' <= c && c <= 'Z')
		return c - 'A';
	if ('a' <= c && c <= 'z')
		return c - 'a' + 26;
	if ('0' <= c && c <= '9')
		return c - '0' + 52;
	return c == '+' ? 62 : 63;
}

int
base64_enc(int insize, uchar *in, int outsize, uchar *out)
{
	uint32_t buf;
	int i, o;

	o = 0;
	for (i = 0; i + 3 <= insize; i += 3) {
		if (o + 4 > outsize)
			return -1;
		buf = 0;
		for (int j = 0; j < 3; ++j)
			buf |= in[i + j] << (2 - j) * 8;
		for (int j = 0; j < 4; ++j)
			out[o++] = int2base64(buf >> (3 - j) * 6 & 0b111111);
	}
	if (insize - i == 1) {
		buf = in[i] << 4;
		if (o + 4 > outsize)
			return -1;
		out[o++] = int2base64(buf >> 6 & 0b111111);
		out[o++] = int2base64(buf & 0b111111);
		out[o++] = '=';
		out[o++] = '=';
	} else if (insize - i == 2) {
		buf = in[i] << 10 | in[i + 1] << 2;
		if (o + 4 > outsize)
			return -1;
		out[o++]   = int2base64(buf >> 12 & 0b111111);
		out[o++] = int2base64(buf >> 6  & 0b111111);
		out[o++] = int2base64(buf & 0b111111);
		out[o++] = '=';
	}
	return o;
}

int
base64_dec(int insize, uchar *in, int outsize, uchar *out)
{
	uint32_t buf;
	int i, o;

	o = 0;
	for (i = 0; i < insize; i += 4) {
		buf = 0;
		if (o + 3 > outsize)
			return -1;
		for (int j = 0; j < 4; ++j) {
			if (in[j] != '=')
				buf |= base642int(in[j]) << (3 - j) * 6;
		}
		for (int j = 0; j < 3; ++j)
			out[o++] = buf >> (2 - j) * 8 & 0xFF;
	}
	return o;
}
