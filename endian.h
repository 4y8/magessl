#ifndef _MAGESSL_ENDIAN_H_
#define _MAGESSL_ENDIAN_H_

#include <stdint.h>

/* GCC warnings want me to put parentheses around the substraction, event if
 * it's useless according to the C standard... */
#define ROTL32(x, n)	(((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR32(x, n)	(((x) << (32 - (n))) | ((x) >> (n)))
#define ROTL64(x, n)	(((x) << (n)) | ((x) >> (64 - (n))))
#define ROTR64(x, n)	(((x) << (64 - (n))) | ((x) >> (n)))

typedef unsigned char uchar;
typedef uint64_t ulong;
typedef uint32_t uint;

void dec32le(uchar in[4], uint *out);
void enc32le(uint in, uchar out[4]);

void dec_blk32le(uchar *in, ulong size, uint *out);
void enc_blk32le(uint *in, ulong size, uchar *out);

void dec32be(uchar in[4], uint *out);
void enc32be(uint in, uchar out[4]);

void dec_blk32be(uchar *in, ulong size, uint *out);
void enc_blk32be(uint *in, ulong size, uchar *out);

void enc64le(ulong in, uchar out[8]);

void enc_blk64le(ulong *in, ulong size, uchar *out);

void dec64be(uchar in[8], ulong *out);
void enc64be(ulong in, uchar out[8]);

#endif
