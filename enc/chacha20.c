#include <stdint.h>
#include <string.h>

#include "enc.h"
#include "endian.h"

#define QROUND(a, b, c, d) \
	w[a] += w[b]; w[d] = ROTL32(w[d] ^ w[a], 16); \
	w[c] += w[d]; w[b] = ROTL32(w[b] ^ w[c], 12); \
	w[a] += w[b]; w[d] = ROTL32(w[d] ^ w[a], 8); \
	w[c] += w[d]; w[b] = ROTL32(w[b] ^ w[c], 7)

static uint32_t init[4] = {
	0x61707865, 0x3320646E, 0x79622D32, 0x6B206574
};

static void
chacha20_inner_block(uint32_t w[16])
{
	QROUND(0, 4, 8,  12);
	QROUND(1, 5, 9,  13);
	QROUND(2, 6, 10, 14);
	QROUND(3, 7, 11, 15);

	QROUND(0, 5, 10, 15);
	QROUND(1, 6, 11, 12);
	QROUND(2, 7, 8,  13);
	QROUND(3, 4, 9,  14);
}

static void
chacha20_block(uint32_t key[8], uint32_t nonce[3], uint32_t cc, uchar out[64])
{
	uint32_t w[16];
	uint32_t ws[16];

	memcpy(w, init, 4 * sizeof(uint32_t));
	memcpy(w + 4, key, 8 * sizeof(uint32_t));
	memcpy(w + 12, &cc, sizeof(uint32_t));
	memcpy(w + 13, nonce, 3 * sizeof(uint32_t));

	memcpy(ws, w, 16 * sizeof(uint32_t));

	for (int i = 0; i < 10; ++i)
		chacha20_inner_block(ws);

	for (int i = 0; i < 16; ++i)
		w[i] += ws[i];
	enc_blk32le(w, 16, out);
}

void
chacha20_enc(uint32_t key[8], uint32_t cc, uint32_t nonce[3],
             ulong len, uchar *in, uchar *out)
{
	ulong i;
	uchar key_stream[64];

	for (i = 0; i < (len >> 6); ++i) {
		chacha20_block(key, nonce, cc + i, key_stream);
		for (int j = 0; j < 64; ++j)
			out[(i << 6) + j] = in[(i << 6) + j] ^ key_stream[j];

	}
	if (len % 64 != 0) {
		i = len / 64;
		chacha20_block(key, nonce, cc + i, key_stream);
		for (int j = 0; j < len % 64; ++j)
			out[(i << 6) + j] = in[(i << 6) + j] ^ key_stream[j];
	}
}
