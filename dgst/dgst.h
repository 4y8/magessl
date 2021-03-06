#ifndef _MAGESSL_DGST_H_
#define _MAGESSL_DGST_H_

#include <stdint.h>

int md5_dgst(uint64_t insize, unsigned char *in, uint32_t out[4]);
int sha1_dgst(uint64_t insize, unsigned char *in, uint32_t out[5]);

#endif
