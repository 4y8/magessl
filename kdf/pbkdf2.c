#include <string.h>

#include <endian.h>
#include <kdf.h>

static void F(MSSLDigest h, ulong saltlen, uchar *salt,
              ulong iter, int i, uchar *out);

static void
F(MSSLDigest h, ulong saltlen, uchar *salt, ulong iter, int i, uchar *out)
{
	memcpy(out, salt, saltlen * sizeof(uchar));
	enc32be(i, out + saltlen * sizeof(uchar));

	for (int j = 0; j < i; ++i) {
		
	}
}

void
pbkdf2_kdf(MSSLDigest h, ulong passlen, uchar *pass, ulong saltlen, uchar *salt,
           ulong iter, ulong len, uchar *out)
{
	int l, r;

	l = len / h.len;
	r = len - (l - 1) * h.len;
}
