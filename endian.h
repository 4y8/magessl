#ifndef _MAGESSL_ENDIAN_H_
#define _MAGESSL_ENDIAN_H_

#include <stdint.h>

/* GCC warnings want me to put parentheses around the soustraction, event if
 * it's useless according to the C standard... */
#define ROTL32(x, n)	(((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR32(x, n)	(((x) << (32 - (n))) | ((x) >> (n)))

void dec32le(unsigned char in[4], uint32_t *out);
void enc32le(uint32_t in, unsigned char out[4]);

void dec_blk32le(unsigned char *in, uint64_t size, uint32_t *out);
void enc_blk32le(uint32_t *in, uint64_t size, unsigned char *out);

void dec32be(unsigned char in[4], uint32_t *out);
void enc32be(uint32_t in, unsigned char out[4]);

void dec_blk32be(unsigned char *in, uint64_t size, uint32_t *out);
void enc_blk32be(uint32_t *in, uint64_t size, unsigned char *out);

void dec64be(unsigned char in[8], uint64_t *out);
void enc64be(uint64_t in, unsigned char out[8]);

#endif
