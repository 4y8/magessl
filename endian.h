#ifndef _MAGESSL_ENDIAN_H_
#define _MAGESSL_ENDIAN_H_

#include <stdint.h>

void dec32le(unsigned char in[4], uint32_t *out);
void enc32le(uint32_t in, unsigned char out[4]);

void dec_blk32le(unsigned char *in, uint64_t size, uint32_t *out);
void enc_blk32le(uint32_t *in, uint64_t size, unsigned char *out);

#endif
