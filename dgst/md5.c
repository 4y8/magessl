#include <stdint.h>

static uint32_t K[64] = {
	0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
	0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501
};

int
md5_dgst(int insize, unsigned char *in, int outsize, usigned char *out)
{
	uint32_t a, b, c, d;

	a = 0x67452301;
	b = 0xEFCDAB89;
	c = 0x98BADCFE;
	d = 0x10325476;
}
