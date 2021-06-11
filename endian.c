#include <endian.h>

void
dec32le(uchar in[4], uint *out)
{
	*out = (uint)in[0]
		| (uint)in[1] << 8
		| (uint)in[2] << 16
		| (uint)in[3] << 24;
}

void
enc32le(uint in, uchar out[4])
{
	out[0] = (uchar)in;
	out[1] = (uchar)(in >> 8);
	out[2] = (uchar)(in >> 16);
	out[3] = (uchar)(in >> 24);
}

void
dec_blk32le(uchar *in, ulong size, uint *out)
{
	for (int i = 0; i < size; i += 4)
		dec32le(in + i, out + (i >> 2));
}

void
enc_blk32le(uint *in, ulong size, uchar *out)
{
	for (int i = 0; i < size; ++i)
		enc32le(in[i], out + (i << 2));
}

void
dec32be(uchar in[4], uint *out)
{
	*out = (uint)in[0] << 24
		| (uint)in[1] << 16
		| (uint)in[2] << 8
		| (uint)in[3];
}

void
enc32be(uint in, uchar out[4])
{
	out[0] = (uchar)(in >> 24);
	out[1] = (uchar)(in >> 16);
	out[2] = (uchar)(in >> 8);
	out[3] = (uchar)in;

}

void
dec_blk32be(uchar *in, ulong size, uint *out)
{
	for (int i = 0; i < size; i += 4)
		dec32be(in + i, out + (i >> 2));
}

void
enc_blk32be(uint *in, ulong size, uchar *out)
{
	for (int i = 0; i < size; ++i)
		enc32be(in[i], out + (i << 2));
}

void
enc64le(ulong in, uchar out[8])
{
	enc32le((uint)in, out);
	enc32le((uint)(in >> 32), out + 4);
}

void
dec64be(uchar in[8], ulong *out)
{
	dec32be(in, (uint *)out);
	dec32be(in + 4, (uint *)(out + 4));
}

void
enc64be(ulong in, uchar out[8])
{
	enc32be((uint)(in >> 32), out);
	enc32be((uint)(in), out + 4);
}

void
enc_blk64le(ulong *in, ulong size, uchar *out)
{
	for (int i = 0; i < size; ++i)
		enc64le(in[i], out + (i << 3));
}
