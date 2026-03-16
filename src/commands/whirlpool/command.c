/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 23:27:48 by rperez-t          #+#    #+#             */
/*   Updated: 2026/03/16 21:07:08 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

void	whirlpool_init_round_key(int round, uint64_t (*c)[256], uint64_t *rc);

static t_whirlpool_tables	g_whirlpool_tables = {
{
	6179, 50920, 34744, 335, 13990, 54005, 31087, 37202,
	24764, 39822, 41740, 31541, 7648, 55234, 11851, 65111,
	5495, 14309, 40944, 19162, 22729, 10506, 45472, 27525,
	48477, 4340, 52030, 1383, 58407, 16779, 42877, 38360,
	64494, 31846, 56599, 18334, 51757, 48903, 44378, 33587,
	25346, 43633, 51225, 18905, 62179, 23432, 39462, 12976,
	59663, 54656, 48845, 13384, 65402, 36959, 8296, 6830,
	46164, 37666, 25841, 29458, 16392, 50156, 56225, 36157,
	38656, 53035, 30338, 54811, 46511, 27216, 17907, 12527,
	16213, 41706, 26042, 12224, 56860, 64845, 37493, 1674,
	45798, 3615, 25300, 43158, 63941, 9561, 33906, 14668,
	24184, 14476, 53669, 57953, 45857, 39966, 17351, 64516,
	20889, 27917, 64223, 32292, 15275, 52753, 36686, 47083,
	15489, 38135, 47379, 11475, 59246, 50179, 22084, 32681,
	10939, 49491, 56331, 40300, 12660, 63046, 44169, 5345,
	5690, 26889, 28854, 53485, 52290, 39076, 10332, 63622
},
{{0}},
{0},
	0
};

static t_whirlpool_tables	*whirlpool_tables(void)
{
	return (&g_whirlpool_tables);
}

static uint64_t	build_entry(int x)
{
	t_whirlpool_tables	*tables;
	uint32_t			v[6];
	uint16_t			c;
	uint64_t			entry;

	tables = whirlpool_tables();
	c = tables->sbox_words[x / 2];
	if ((x % 2) == 0)
		v[0] = c >> 8;
	else
		v[0] = c & 0xFF;
	v[1] = v[0] << 1;
	if (v[1] >= 0x100)
		v[1] ^= 0x11D;
	v[2] = v[1] << 1;
	if (v[2] >= 0x100)
		v[2] ^= 0x11D;
	v[3] = v[2] ^ v[0];
	v[4] = v[2] << 1;
	if (v[4] >= 0x100)
		v[4] ^= 0x11D;
	v[5] = v[4] ^ v[0];
	entry = ((uint64_t)v[0] << 56) | ((uint64_t)v[0] << 48);
	entry |= ((uint64_t)v[2] << 40) | ((uint64_t)v[0] << 32);
	entry |= ((uint64_t)v[4] << 24) | ((uint64_t)v[3] << 16);
	entry |= ((uint64_t)v[1] << 8) | (uint64_t)v[5];
	return (entry);
}

void	whirlpool_init_tables(void)
{
	t_whirlpool_tables	*tables;
	int					x;
	int					idx;
	int					round;

	tables = whirlpool_tables();
	if (tables->initialized)
		return ;
	x = -1;
	while (++x < 256)
	{
		tables->c[0][x] = build_entry(x);
		idx = 0;
		while (++idx < 8)
			tables->c[idx][x] = (tables->c[idx - 1][x] >> 8)
				| (tables->c[idx - 1][x] << 56);
	}
	tables->rc[0] = 0;
	round = 0;
	while (++round <= 10)
		whirlpool_init_round_key(round, tables->c, tables->rc);
	tables->initialized = 1;
}

uint64_t	whirlpool_transform(const uint64_t src[8], int index)
{
	t_whirlpool_tables	*tables;
	uint64_t			value;
	int					shift;
	int					table;

	tables = whirlpool_tables();
	value = 0;
	shift = 56;
	table = 0;
	while (table < 8)
	{
		value ^= tables->c[table][(src[(index - table) & 7] >> shift) & 0xFF];
		table++;
		shift -= 8;
	}
	return (value);
}

uint64_t	whirlpool_round_constant(int round)
{
	return (whirlpool_tables()->rc[round]);
}
