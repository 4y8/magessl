#include <string.h>

#include <dgst.h>
#include <endian.h>

int
hmac_dgst(MSSLDigest h, ulong keysize, uchar *key,
          ulong insize, uchar *in, uchar *out)
{
	uchar nkey[h.blocksize];
	uchar bufi[h.blocksize + insize], bufo[h.blocksize + h.outsize];

	memset(nkey, 0, h.blocksize);
	if (keysize > h.blocksize) {
		h.f(keysize, key, nkey);
	} else {
		memcpy(nkey, key, keysize);
	}
	for (int i = 0; i < h.blocksize; ++i) {
		bufo[i] = nkey[i] ^ 0x5c;
		bufi[i] = nkey[i] ^ 0x36;
	}
	memcpy(bufi + h.blocksize, in, insize);
	h.f(h.blocksize + insize, bufi, bufo + h.blocksize);
	h.f(h.blocksize + h.outsize, bufo, out);

	return 0;
}
