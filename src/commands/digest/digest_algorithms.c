#include "ft_ssl.h"

static const t_digest_algorithm	g_md5 = {
	"md5",
	"MD5",
	16,
	get_md5_algorithm()->compute
};

static const t_digest_algorithm	g_sha256 = {
	"sha256",
	"SHA256",
	32,
	get_sha256_algorithm()->compute
};

const t_digest_algorithm	*get_md5_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"md5",
		"MD5",
		16,
		NULL
	};
	return (&algorithm);
}

const t_digest_algorithm	*get_sha256_algorithm(void)
{
	static const t_digest_algorithm	algorithm = {
		"sha256",
		"SHA256",
		32,
		NULL
	};
	return (&algorithm);
}
