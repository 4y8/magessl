#include <string.h>

#include <dgst.h>
#include <endian.h>

int
hmac_dgst(MSSLDigest h, ulong keysize, uchar *key,
          ulong insize, uchar in, uchar *out)
{
	uchar nkey[h.blocksize];
	uchar okeypad[h.blocksize], ikeypad[h.blocksize];

	memset(nkey, 0, h.blocksize);
	if (keysize > h.blocksize) {
		h.f(keysize, key, nkey);
	} else {
		memcpy(nkey, key, keysize);
	}
	for (int i = 0; i < h.blocksize; ++i) {
		okeypad[i] = nkey[i] ^ 0x5c;
		ikeypad[i] = nkey[i] ^ 0x36;
	}
	

}
