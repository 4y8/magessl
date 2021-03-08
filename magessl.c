#include <stdio.h>
#include <string.h>

#include "error.h"

#include "dgst/dgst.h"
#include "enc/enc.h"

static enum mssl_ciphers ciphername(char *s);
static enum mssl_digests digestname(char *s);

static void enc(int argc, char **argv);
static void dec(int argc, char **argv);
static void dgst(int argc, char **argv);

static struct {
	char *name;
	void (*cmd)(int argc, char **argv);
} cmds[] = {{"enc",   enc},
            {"dec",   dec},
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
	i = 0;
	dflag = 0;
	while (argc-- && (*argv)[i] == '-') {
		p = (*(argv++)) + ++i;
		if (!strcmp(p, "d"))
			dflag = 1;
		else if ((tmpcipher = ciphername(p)) > 0)
			cipher = tmpcipher;
		else
			error("invalid option: -%s", p);
	}
	if (cipher < 0)
		error("no cipher given");
	if (argc != 1)
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
			error("couldn't run cipher");
		for (int i = 0; i < ret; ++i)
			putchar(out[i]);
		putchar('\n');
		break;
	}
	}
}

static void
dec(int argc, char **argv)
{
	char *p;
	unsigned char *up;
	int i, len, ret;
	enum mssl_ciphers cipher, tmpcipher;

	cipher = -1;
	i = 0;
	while (argc-- && (*argv)[i] == '-') {
		p = (*(argv++)) + ++i;
		if ((tmpcipher = ciphername(p)) > 0)
			cipher = tmpcipher;
		else
			error("invalid option: -%s", p);
	}
	if (cipher < 0)
		error("no cipher given");
	if (argc != 1)
		error("invalid number of arguments");
	up = (unsigned char *)argv[i];
	len = strlen(argv[i]);
	switch (cipher) {
	case MSSL_BASE64: {
		int outlen = 4 / 3 * (len + 2);
		unsigned char out[outlen];
		ret = base64_dec(len, up, outlen, out);
		if (ret < 0)
			error("couldn't run cipher");
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

	if (!strcmp(argv[1], "dgst")) {
		unsigned char buf[16];
		md5_dgst(strlen(argv[3]), (unsigned char *)argv[3], buf);
		for (int i = 0; i < 16; ++i)
			printf("%02x", buf[i]);
		printf("\n");
	}
}
