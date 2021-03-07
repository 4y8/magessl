#include <stdio.h>
#include <string.h>

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
	enum mssl_ciphers cipher;
	int dflag;

	cipher = -1;
	while (argc-- && (*argv)[0] == '-') {
		
	}
}

static void dec(int argc, char **argv);
static void dgst(int argc, char **argv);

int
main(int argc, char **argv)
{
	int dflag;

	dflag = 0;
	if (argc == 1)
		return 1;
	if (!strcmp(argv[1], "enc")) {
		if (!strcmp(argv[2], "-base64")) {
			unsigned char buf[256];
			int ret;
			if (dflag)
				ret = base64_dec(strlen(argv[3]), (unsigned char *)argv[3], 256, buf);
			else
				ret = base64_enc(strlen(argv[3]), (unsigned char *)argv[3], 256, buf);
			for (int i = 0; i < ret; ++i)
				putchar(buf[i]);
			putchar('\n');
		}
	} else if (!strcmp(argv[1], "dec")) {
		if (!strcmp(argv[2], "-base64")) {
			unsigned char buf[256];
			int ret;
			ret = base64_dec(strlen(argv[3]), argv[3], 256, buf);
			for (int i = 0; i < ret; ++i)
				putchar(buf[i]);
			putchar('\n');
		}
	} else if (!strcmp(argv[1], "dgst")) {
		unsigned char buf[16];
		md5_dgst(strlen(argv[3]), (unsigned char *)argv[3], buf);
		for (int i = 0; i < 16; ++i)
			printf("%02x", buf[i]);
		printf("\n");
	}
}
