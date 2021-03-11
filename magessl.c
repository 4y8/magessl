#include <stdio.h>
#include <string.h>

#include "error.h"

#include "dgst/dgst.h"
#include "enc/enc.h"

static enum mssl_ciphers ciphername(char *s);
static enum mssl_digests digestname(char *s);

static void enc(int argc, char **argv);
static void dgst(int argc, char **argv);

static struct {
	char *name;
	void (*cmd)(int argc, char **argv);
} cmds[] = {{"enc",   enc},
            {"dgst", dgst}};

static enum mssl_ciphers
ciphername(char *s)
{
	if (!strcmp(s, "base64"))
		return MSSL_BASE64;
	return -1;
}

static enum mssl_digests
digestname(char *s)
{
	if (!strcmp(s, "md5"))
		return MSSL_MD5;
	else if (!strcmp(s, "sha1"))
		return MSSL_SHA1;
	else if (!strcmp(s, "md4"))
		return MSSL_MD4;
	return -1;
}

static void
enc(int argc, char **argv)
{
	char *p;
	unsigned char *up;
	int dflag, i, len, ret;
	enum mssl_ciphers cipher, tmpcipher;

	cipher = -1;
	dflag = 0;
	for (i = 0; argc && argv[i][0] == '-'; --argc) {
		p = argv[i++] + 1;
		if (!strcmp(p, "d"))
			dflag = 1;
		else if ((tmpcipher = ciphername(p)) >= 0)
			cipher = tmpcipher;
		else
			error("invalid option: -%s", p);
	}
	if (cipher < 0)
		error("no cipher given");
	if (argc < 1)
		error("invalid number of arguments");
	up = (unsigned char *)argv[i];
	len = strlen(argv[i]);
	switch (cipher) {
	case MSSL_BASE64: {
		int outlen = 4 / 3 * (len + 2);
		unsigned char out[outlen];
		if (dflag)
			ret = base64_dec(len, up, outlen, out);
		else
			ret = base64_enc(len, up, outlen, out);
		if (ret < 0)
			error("couldn't run cipher\n");
		for (int i = 0; i < ret; ++i)
			putchar(out[i]);
		putchar('\n');
		break;
	}
	}
}

static void
dgst(int argc, char **argv)
{
	char *p;
	unsigned char *up;
	int i, len;
	enum mssl_digests digest, tmpdgst;

	digest = -1;
	for (i = 0; argc && argv[i][0] == '-'; --argc) {
		p = argv[i++] + 1;
		if ((tmpdgst = digestname(p)) >= 0)
			digest = tmpdgst;
		else
			error("invalid option: -%s", p);
	}
	if (digest < 0)
		error("no digest given");
	if (argc < 1)
		error("invalid number of arguments");
	up = (unsigned char *)argv[i];
	len = strlen(argv[i]);
	switch (digest) {
	case MSSL_MD5: {
		unsigned char out[16];
		if (md5_dgst(len, up, out) < 0)
			error("digest failed");

		for (int i = 0; i < 16; ++i)
			printf("%02x", out[i]);
		printf("\n");
		break;
	}
	case MSSL_SHA1: {
		unsigned char out[20];
		if (sha1_dgst(len, up, out) < 0)
			error("digest failed");

		for (int i = 0; i < 20; ++i)
			printf("%02x", out[i]);
		printf("\n");
		break;
	}
	case MSSL_MD4: {
		unsigned char out[16];
		if (md4_dgst(len, up, out) < 0)
			error("digest failed");

		for (int i = 0; i < 16; ++i)
			printf("%02x", out[i]);
		printf("\n");
		break;
	}
	}
}

int
main(int argc, char **argv)
{

	if (argc == 1)
		return 1;

	for (int i = 0; i < sizeof(cmds) / sizeof(cmds[0]); ++i)
		if (!strcmp(argv[1], cmds[i].name))
			cmds[i].cmd(argc - 2, argv + 2);
	return 1;
}
