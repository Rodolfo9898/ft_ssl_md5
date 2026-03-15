#include "ft_ssl.h"

typedef struct s_wp_word
{
	uint32_t	v[2];
} 	t_wp_word;

static const uint16_t	g_sbox_words[128] = {
	0x1823, 0xc6e8, 0x87b8, 0x014f, 0x36a6, 0xd2f5, 0x796f, 0x9152,
	0x60bc, 0x9b8e, 0xa30c, 0x7b35, 0x1de0, 0xd7c2, 0x2e4b, 0xfe57,
	0x1577, 0x37e5, 0x9ff0, 0x4ada, 0x58c9, 0x290a, 0xb1a0, 0x6b85,
	0xbd5d, 0x10f4, 0xcb3e, 0x0567, 0xe427, 0x418b, 0xa77d, 0x95d8,
	0xfbee, 0x7c66, 0xdd17, 0x479e, 0xca2d, 0xbf07, 0xad5a, 0x8333,
	0x6302, 0xaa71, 0xc819, 0x49d9, 0xf2e3, 0x5b88, 0x9a26, 0x32b0,
	0xe90f, 0xd580, 0xbecd, 0x3448, 0xff7a, 0x905f, 0x2068, 0x1aae,
	0xb454, 0x9322, 0x64f1, 0x7312, 0x4008, 0xc3ec, 0xdba1, 0x8d3d,
	0x9700, 0xcf2b, 0x7682, 0xd61b, 0xb5af, 0x6a50, 0x45f3, 0x30ef,
	0x3f55, 0xa2ea, 0x65ba, 0x2fc0, 0xde1c, 0xfd4d, 0x9275, 0x068a,
	0xb2e6, 0x0e1f, 0x62d4, 0xa896, 0xf9c5, 0x2559, 0x8472, 0x394c,
	0x5e78, 0x388c, 0xd1a5, 0xe261, 0xb321, 0x9c1e, 0x43c7, 0xfc04,
	0x5199, 0x6d0d, 0xfadf, 0x7e24, 0x3bab, 0xce11, 0x8f4e, 0xb7eb,
	0x3c81, 0x94f7, 0xb913, 0x2cd3, 0xe76e, 0xc403, 0x5644, 0x7fa9,
	0x2abb, 0xc153, 0xdc0b, 0x9d6c, 0x3174, 0xf646, 0xac89, 0x14e1,
	0x163a, 0x6909, 0x70b6, 0xd0ed, 0xcc42, 0x98a4, 0x285c, 0xf886
};

static uint32_t	g_c[8][256][2];
static uint32_t	g_rc[11][2];
static int		g_initialized;

static void	init_tables(void)
{
	uint32_t	v1;
	uint32_t	v2;
	uint32_t	v4;
	uint32_t	v5;
	uint32_t	v8;
	uint32_t	v9;
	uint16_t	c;
	int			t;
	int			x;
	int			r;
	int			i;

	if (g_initialized)
		return ;
	t = 0;
	while (t < 8)
	{
		x = 0;
		while (x < 256)
		{
			g_c[t][x][0] = 0;
			g_c[t][x][1] = 0;
			x++;
		}
		t++;
	}
	x = 0;
	while (x < 256)
	{
		c = g_sbox_words[x / 2];
		v1 = ((x & 1) == 0) ? (uint32_t)(c >> 8) : (uint32_t)(c & 0xff);
		v2 = v1 << 1;
		if (v2 >= 0x100)
			v2 ^= 0x11d;
		v4 = v2 << 1;
		if (v4 >= 0x100)
			v4 ^= 0x11d;
		v5 = v4 ^ v1;
		v8 = v4 << 1;
		if (v8 >= 0x100)
			v8 ^= 0x11d;
		v9 = v8 ^ v1;
		g_c[0][x][0] = (v1 << 24) | (v1 << 16) | (v4 << 8) | v1;
		g_c[0][x][1] = (v8 << 24) | (v5 << 16) | (v2 << 8) | v9;
		t = 1;
		while (t < 8)
		{
			g_c[t][x][0] = (g_c[t - 1][x][0] >> 8)
				| (g_c[t - 1][x][1] << 24);
			g_c[t][x][1] = (g_c[t - 1][x][1] >> 8)
				| (g_c[t - 1][x][0] << 24);
			t++;
		}
		x++;
	}
	g_rc[0][0] = 0;
	g_rc[0][1] = 0;
	r = 1;
	while (r <= 10)
	{
		i = 8 * (r - 1);
		g_rc[r][0] = (g_c[0][i][0] & 0xff000000U)
			^ (g_c[1][i + 1][0] & 0x00ff0000U)
			^ (g_c[2][i + 2][0] & 0x0000ff00U)
			^ (g_c[3][i + 3][0] & 0x000000ffU);
		g_rc[r][1] = (g_c[4][i + 4][1] & 0xff000000U)
			^ (g_c[5][i + 5][1] & 0x00ff0000U)
			^ (g_c[6][i + 6][1] & 0x0000ff00U)
			^ (g_c[7][i + 7][1] & 0x000000ffU);
		r++;
	}
	g_initialized = 1;
}

static void	process_buffer(uint32_t hash[8][2], const unsigned char *buffer)
{
	uint32_t	block[8][2];
	uint32_t	k[8][2];
	uint32_t	l[8][2];
	uint32_t	state[8][2];
	int			i;
	int			j;
	int			r;
	int			s;
	int			t;

	i = 0;
	j = 0;
	while (i < 8)
	{
		block[i][0] = ((uint32_t)buffer[j] << 24)
			^ ((uint32_t)buffer[j + 1] << 16)
			^ ((uint32_t)buffer[j + 2] << 8)
			^ (uint32_t)buffer[j + 3];
		block[i][1] = ((uint32_t)buffer[j + 4] << 24)
			^ ((uint32_t)buffer[j + 5] << 16)
			^ ((uint32_t)buffer[j + 6] << 8)
			^ (uint32_t)buffer[j + 7];
		state[i][0] = block[i][0] ^ (k[i][0] = hash[i][0]);
		state[i][1] = block[i][1] ^ (k[i][1] = hash[i][1]);
		i++;
		j += 8;
	}
	r = 1;
	while (r <= 10)
	{
		i = 0;
		while (i < 8)
		{
			l[i][0] = 0;
			l[i][1] = 0;
			t = 0;
			s = 56;
			j = 0;
			while (t < 8)
			{
				l[i][0] ^= g_c[t][(k[(i - t) & 7][j] >> (s % 32)) & 0xff][0];
				l[i][1] ^= g_c[t][(k[(i - t) & 7][j] >> (s % 32)) & 0xff][1];
				t++;
				s -= 8;
				j = (s < 32);
			}
			i++;
		}
		i = 0;
		while (i < 8)
		{
			k[i][0] = l[i][0];
			k[i][1] = l[i][1];
			i++;
		}
		k[0][0] ^= g_rc[r][0];
		k[0][1] ^= g_rc[r][1];
		i = 0;
		while (i < 8)
		{
			l[i][0] = k[i][0];
			l[i][1] = k[i][1];
			t = 0;
			s = 56;
			j = 0;
			while (t < 8)
			{
				l[i][0] ^= g_c[t][(state[(i - t) & 7][j] >> (s % 32)) & 0xff][0];
				l[i][1] ^= g_c[t][(state[(i - t) & 7][j] >> (s % 32)) & 0xff][1];
				t++;
				s -= 8;
				j = (s < 32);
			}
			i++;
		}
		i = 0;
		while (i < 8)
		{
			state[i][0] = l[i][0];
			state[i][1] = l[i][1];
			i++;
		}
		r++;
	}
	i = 0;
	while (i < 8)
	{
		hash[i][0] ^= state[i][0] ^ block[i][0];
		hash[i][1] ^= state[i][1] ^ block[i][1];
		i++;
	}
}

static void	build_length(unsigned char bit_length[32], size_t len)
{
	uint64_t	bits;
	int			index;

	ft_memset(bit_length, 0, 32);
	bits = (uint64_t)len * 8;
	index = 0;
	while (index < 8)
	{
		bit_length[31 - index] = (unsigned char)(bits & 0xff);
		bits >>= 8;
		index++;
	}
}

static int	whirlpool_compute(const unsigned char *data, size_t len,
	unsigned char *digest)
{
	uint32_t		hash[8][2];
	unsigned char	block[64];
	unsigned char	bit_length[32];
	size_t			offset;
	size_t			remaining;
	int				index;

	init_tables();
	index = 0;
	while (index < 8)
	{
		hash[index][0] = 0;
		hash[index][1] = 0;
		index++;
	}
	offset = 0;
	while (offset + 64 <= len)
	{
		process_buffer(hash, data + offset);
		offset += 64;
	}
	remaining = len - offset;
	ft_memset(block, 0, sizeof(block));
	if (remaining > 0)
		ft_memcpy(block, data + offset, remaining);
	block[remaining] |= 0x80;
	if (remaining + 1 > 32)
	{
		process_buffer(hash, block);
		ft_memset(block, 0, sizeof(block));
	}
	build_length(bit_length, len);
	ft_memcpy(block + 32, bit_length, 32);
	process_buffer(hash, block);
	index = 0;
	while (index < 8)
	{
		digest[index * 8] = (unsigned char)(hash[index][0] >> 24);
		digest[index * 8 + 1] = (unsigned char)(hash[index][0] >> 16);
		digest[index * 8 + 2] = (unsigned char)(hash[index][0] >> 8);
		digest[index * 8 + 3] = (unsigned char)hash[index][0];
		digest[index * 8 + 4] = (unsigned char)(hash[index][1] >> 24);
		digest[index * 8 + 5] = (unsigned char)(hash[index][1] >> 16);
		digest[index * 8 + 6] = (unsigned char)(hash[index][1] >> 8);
		digest[index * 8 + 7] = (unsigned char)hash[index][1];
		index++;
	}
	return (1);
}

const t_digest_algorithm	*get_whirlpool_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"whirlpool",
		"WHIRLPOOL",
		64,
		whirlpool_compute
	};

	return (&algorithm);
}
