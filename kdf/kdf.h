#ifndef _MAGESSL_KDF_H_
#define _MAGESSL_KDF_H_

#include <dgst.h>
#include <endian.h>

void pbkdf2_kdf(MSSLDigest prf, uchar *pass, uchar *salt, ulong iter, ulong len);

#endif
