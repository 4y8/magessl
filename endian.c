#include <stdint.h>

#include "endian.h"


void
dec32le(unsigned char in[4], uint32_t *out)
{
	*out = (uint32_t)in[0]
		| (uint32_t)in[1] << 8
		| (uint32_t)in[2] << 16
		| (uint32_t)in[3] << 24;
}

void
enc32le(uint32_t in, unsigned char out[4])
{
	out[0] = (unsigned char)in;
	out[1] = (unsigned char)(in >> 8);
	out[2] = (unsigned char)(in >> 16);
	out[3] = (unsigned char)(in >> 24);
}

void
dec_blk32le(unsigned char *in, uint64_t size, uint32_t *out)
{
	for (int i = 0; i < size; i += 4)
		dec32le(in + i, out + (i >> 2));
}

void
enc_blk32le(uint32_t *in, uint64_t size, unsigned char *out)
{
	for (int i = 0; i < size; ++i)
		enc32le(in[i], out + (i << 2));
}

void
dec32be(unsigned char in[4], uint32_t *out)
{
	*out = (uint32_t)in[0] << 24
		| (uint32_t)in[1] << 16
		| (uint32_t)in[2] << 8
		| (uint32_t)in[3];
}

void
enc32be(uint32_t in, unsigned char out[4])
{
	out[0] = (unsigned char)(in >> 24);
	out[1] = (unsigned char)(in >> 16);
	out[2] = (unsigned char)(in >> 8);
	out[3] = (unsigned char)in;

}

void
dec_blk32be(unsigned char *in, uint64_t size, uint32_t *out)
{
	for (int i = 0; i < size; i += 4)
		dec32be(in + i, out + (i >> 2));
}

void
enc_blk32be(uint32_t *in, uint64_t size, unsigned char *out)
{
	for (int i = 0; i < size; ++i)
		enc32be(in[i], out + (i << 2));
}

void
dec64be(unsigned char in[8], uint64_t *out)
{
	dec32be(in, (uint32_t *)out);
	dec32be(in + 4, (uint32_t *)(out + 4));
}

void
enc64be(uint64_t in, unsigned char out[8])
{
	enc32be((uint32_t)(in >> 32), out);
	enc32be((uint32_t)(in), out + 4);
}
