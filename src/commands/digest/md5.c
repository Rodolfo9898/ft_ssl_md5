#include "ft_ssl.h"
#include <errno.h>
#include <stdlib.h>

typedef struct s_md5_ctx
{
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;
} 	t_md5_ctx;

static uint32_t	left_rotate(uint32_t value, uint32_t shift)
{
	return ((value << shift) | (value >> (32 - shift)));
}

static uint32_t	read_u32_le(const unsigned char *src)
{
	return ((uint32_t)src[0] | ((uint32_t)src[1] << 8)
		| ((uint32_t)src[2] << 16) | ((uint32_t)src[3] << 24));
}

static void	write_u32_le(unsigned char *dst, uint32_t value)
{
	dst[0] = (unsigned char)(value & 0xFF);
	dst[1] = (unsigned char)((value >> 8) & 0xFF);
	dst[2] = (unsigned char)((value >> 16) & 0xFF);
	dst[3] = (unsigned char)((value >> 24) & 0xFF);
}

static void	process_block(t_md5_ctx *ctx, const unsigned char *block)
{
	static const uint32_t	s[] = {
		7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
		5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
		4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
		6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
	};
	static const uint32_t	k[] = {
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};
	uint32_t			m[16];
	uint32_t			a;
	uint32_t			b;
	uint32_t			c;
	uint32_t			d;
	uint32_t			f;
	uint32_t			g;
	int					i;

	i = 0;
	while (i < 16)
	{
		m[i] = read_u32_le(block + (i * 4));
		i++;
	}
	a = ctx->a;
	b = ctx->b;
	c = ctx->c;
	d = ctx->d;
	i = 0;
	while (i < 64)
	{
		if (i < 16)
		{
			f = (b & c) | ((~b) & d);
			g = (uint32_t)i;
		}
		else if (i < 32)
		{
			f = (d & b) | ((~d) & c);
			g = (uint32_t)((5 * i + 1) % 16);
		}
		else if (i < 48)
		{
			f = b ^ c ^ d;
			g = (uint32_t)((3 * i + 5) % 16);
		}
		else
		{
			f = c ^ (b | (~d));
			g = (uint32_t)((7 * i) % 16);
		}
		f += a + k[i] + m[g];
		a = d;
		d = c;
		c = b;
		b += left_rotate(f, s[i]);
		i++;
	}
	ctx->a += a;
	ctx->b += b;
	ctx->c += c;
	ctx->d += d;
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
		buffer[new_len + index] = (unsigned char)((bit_len >> (index * 8)) & 0xFF);
		index++;
	}
	return (buffer);
}

static int	md5_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	t_md5_ctx		ctx;
	unsigned char	*buffer;
	size_t			padded_len;
	size_t			offset;

	ctx.a = 0x67452301;
	ctx.b = 0xefcdab89;
	ctx.c = 0x98badcfe;
	ctx.d = 0x10325476;
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
	write_u32_le(digest, ctx.a);
	write_u32_le(digest + 4, ctx.b);
	write_u32_le(digest + 8, ctx.c);
	write_u32_le(digest + 12, ctx.d);
	return (1);
}

const t_digest_algorithm	*get_md5_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"md5",
		"MD5",
		16,
		md5_compute
	};

	return (&algorithm);
}
