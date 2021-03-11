#ifndef _MAGESSL_DGST_H_
#define _MAGESSL_DGST_H_

#include <stdint.h>

int md4_dgst(uint64_t insize, unsigned char *in, unsigned char out[16]);
int md5_dgst(uint64_t insize, unsigned char *in, unsigned char out[16]);
int sha1_dgst(uint64_t insize, unsigned char *in, unsigned char out[20]);

enum mssl_digests {
	MSSL_MD4,
	MSSL_MD5,
	MSSL_SHA1
};

#endif
