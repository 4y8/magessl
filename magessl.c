#include <stdio.h>
#include <string.h>

#include "enc/enc.h"

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
				ret = base64_dec(strlen(argv[3]), argv[3], 256, buf);
			else
				ret = base64_enc(strlen(argv[3]), argv[3], 256, buf);
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

	}
}
