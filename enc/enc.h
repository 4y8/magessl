#ifndef _MAGESSL_ENC_H_
#define _MAGESSL_ENC_H_

int base64_enc(int insize, unsigned char *in, int outsize, unsigned char *out);
int base64_dec(int insize, unsigned char *in, int outsize, unsigned char *out);

enum {
	MSSL_BASE64
} encodings;

#endif
