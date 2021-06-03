#ifndef _MAGESSL_ENDIAN_H_
#define _MAGESSL_ENDIAN_H_

#include <stdint.h>

/* GCC warnings want me to put parentheses around the substraction, event if
 * it's useless according to the C standard... */
#define ROTL32(x, n)	(((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR32(x, n)	(((x) << (32 - (n))) | ((x) >> (n)))

typedef unsigned char uchar;
typedef unsigned long ulong;

void dec32le(uchar in[4], uint32_t *out);
void enc32le(uint32_t in, uchar out[4]);

void dec_blk32le(uchar *in, uint64_t size, uint32_t *out);
void enc_blk32le(uint32_t *in, uint64_t size, uchar *out);

void dec32be(uchar in[4], uint32_t *out);
void enc32be(uint32_t in, uchar out[4]);

void dec_blk32be(uchar *in, uint64_t size, uint32_t *out);
void enc_blk32be(uint32_t *in, uint64_t size, uchar *out);

void dec64be(uchar in[8], uint64_t *out);
void enc64be(uint64_t in, uchar out[8]);

#endif
