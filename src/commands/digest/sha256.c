#include "ft_ssl.h"
#include <errno.h>
#include <stdlib.h>

typedef struct s_sha256_ctx
{
	uint32_t	h[8];
} 	t_sha256_ctx;

static uint32_t	right_rotate(uint32_t value, uint32_t shift)
{
	return ((value >> shift) | (value << (32 - shift)));
}

static uint32_t	read_u32_be(const unsigned char *src)
{
	return (((uint32_t)src[0] << 24) | ((uint32_t)src[1] << 16)
		| ((uint32_t)src[2] << 8) | (uint32_t)src[3]);
}

static void	write_u32_be(unsigned char *dst, uint32_t value)
{
	dst[0] = (unsigned char)((value >> 24) & 0xFF);
	dst[1] = (unsigned char)((value >> 16) & 0xFF);
	dst[2] = (unsigned char)((value >> 8) & 0xFF);
	dst[3] = (unsigned char)(value & 0xFF);
}

static void	process_block(t_sha256_ctx *ctx, const unsigned char *block)
{
	static const uint32_t	k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	uint32_t		w[64];
	uint32_t		a;
	uint32_t		b;
	uint32_t		c;
	uint32_t		d;
	uint32_t		e;
	uint32_t		f;
	uint32_t		g;
	uint32_t		h;
	uint32_t		s0;
	uint32_t		s1;
	uint32_t		ch;
	uint32_t		temp1;
	uint32_t		temp2;
	int				i;

	i = 0;
	while (i < 16)
	{
		w[i] = read_u32_be(block + (i * 4));
		i++;
	}
	while (i < 64)
	{
		s0 = right_rotate(w[i - 15], 7) ^ right_rotate(w[i - 15], 18)
			^ (w[i - 15] >> 3);
		s1 = right_rotate(w[i - 2], 17) ^ right_rotate(w[i - 2], 19)
			^ (w[i - 2] >> 10);
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		i++;
	}
	a = ctx->h[0];
	b = ctx->h[1];
	c = ctx->h[2];
	d = ctx->h[3];
	e = ctx->h[4];
	f = ctx->h[5];
	g = ctx->h[6];
	h = ctx->h[7];
	i = 0;
	while (i < 64)
	{
		s1 = right_rotate(e, 6) ^ right_rotate(e, 11) ^ right_rotate(e, 25);
		ch = (e & f) ^ ((~e) & g);
		temp1 = h + s1 + ch + k[i] + w[i];
		s0 = right_rotate(a, 2) ^ right_rotate(a, 13) ^ right_rotate(a, 22);
		temp2 = s0 + ((a & b) ^ (a & c) ^ (b & c));
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
		i++;
	}
	ctx->h[0] += a;
	ctx->h[1] += b;
	ctx->h[2] += c;
	ctx->h[3] += d;
	ctx->h[4] += e;
	ctx->h[5] += f;
	ctx->h[6] += g;
	ctx->h[7] += h;
}

static unsigned char	*pad_message(const unsigned char *data, size_t len,
	size_t *padded_len)
{
	unsigned char	*buffer;
	uint64_t		bit_len;
	size_t			new_len;
	size_t			index;

	new_len = len + 1;
	while ((new_len % 64) != 56)
		new_len++;
	*padded_len = new_len + 8;
	buffer = (unsigned char *)malloc(*padded_len);
	if (buffer == NULL)
	{
		errno = ENOMEM;
		return (NULL);
	}
	ft_memcpy(buffer, data, len);
	buffer[len] = 0x80;
	if (new_len > len + 1)
		ft_memset(buffer + len + 1, 0, new_len - len - 1);
	bit_len = (uint64_t)len * 8;
	index = 0;
	while (index < 8)
	{
		buffer[new_len + (7 - index)] = (unsigned char)(bit_len & 0xFF);
		bit_len >>= 8;
		index++;
	}
	return (buffer);
}

static int	sha256_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	t_sha256_ctx	ctx;
	unsigned char	*buffer;
	size_t			padded_len;
	size_t			offset;
	int				index;

	ctx.h[0] = 0x6a09e667;
	ctx.h[1] = 0xbb67ae85;
	ctx.h[2] = 0x3c6ef372;
	ctx.h[3] = 0xa54ff53a;
	ctx.h[4] = 0x510e527f;
	ctx.h[5] = 0x9b05688c;
	ctx.h[6] = 0x1f83d9ab;
	ctx.h[7] = 0x5be0cd19;
	buffer = pad_message(data, len, &padded_len);
	if (buffer == NULL)
		return (0);
	offset = 0;
	while (offset < padded_len)
	{
		process_block(&ctx, buffer + offset);
		offset += 64;
	}
	free(buffer);
	index = 0;
	while (index < 8)
	{
		write_u32_be(digest + (index * 4), ctx.h[index]);
		index++;
	}
	return (1);
}

const t_digest_algorithm	*get_sha256_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"sha256",
		"SHA256",
		32,
		sha256_compute
	};

	return (&algorithm);
}
