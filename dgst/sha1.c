#include <stdint.h>
#include <string.h>

#include "dgst.h"

#define F(B, C, D)	((((C) ^ (D)) & (B)) ^ (D))
#define G(B, C, D)	((B) ^ (C) ^ (D))
#define H(B, C, D)	(((D) & (C)) | (((D) | (C)) & (B)))
#define I(B, C, D)	((B) ^ (C) ^ (D))
#define ROTL32(x, n)	(((x) << (n)) | ((x) >> (32 - (n))))
