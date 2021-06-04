#ifndef _MAGESSL_KDF_H_
#define _MAGESSL_KDF_H_

#include <dgst.h>
#include <endian.h>

void pbkdf2_kdf(MSSLDigest h, ulong passlen, uchar *pass, ulong saltlen,
                uchar *salt, ulong iter, ulong len, uchar *out);

#endif
