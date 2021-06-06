#include <string.h>

#include <endian.h>
#include <kdf.h>

static void F(MSSLDigest h, ulong keysize, uchar *key, ulong saltsize,
              uchar *salt, ulong iter, uchar *out);

static void
F(MSSLDigest h, ulong keysize, uchar *key, ulong saltsize,
  uchar *salt, ulong iter, uchar *out)
{
	uchar buf[saltsize + 4], outbuf[h.outsize];

	memcpy(buf, salt, saltsize);
	enc32be(iter, buf + saltsize);

	memset(out, 0, h.outsize);

	for (; iter > 0; --iter) {
		hmac_dgst(h, keysize, key, h.outsize, buf, outbuf);
		memcpy(buf, outbuf, h.outsize);

		for (int i = 0; i < h.outsize; ++i)
			out[i] ^= outbuf[i];
	}
}

void
pbkdf2_kdf(MSSLDigest h, ulong passsize, uchar *pass, ulong saltsize,
           uchar *salt, ulong iter, ulong len, uchar *out)
{
	int l, r;

	l = len / h.outsize;
	r = len - (l - 1) * h.outsize;
}
