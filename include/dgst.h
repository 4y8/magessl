#ifndef _MAGESSL_DGST_H_
#define _MAGESSL_DGST_H_

#include <stdint.h>
#include <endian.h>

int md4_dgst(ulong insize, uchar *in, uchar out[16]);
int md5_dgst(ulong insize, uchar *in, uchar out[16]);
int sha1_dgst(ulong insize, uchar *in, uchar out[20]);
int sha224_dgst(ulong insize, uchar *in, uchar out[32]);
int sha256_dgst(ulong insize, uchar *in, uchar out[32]);
int ripemd_dgst(ulong insize, uchar *in, uchar out[20]);

enum mssl_digests {
	MSSL_MD4,
	MSSL_MD5,
	MSSL_SHA1,
	MSSL_SHA224,
	MSSL_SHA256,
	MSSL_RIPEMD
};

typedef struct {
	int (*f)(ulong, uchar *, uchar *);
	int len;
} MSSLDigest;

#endif
