#ifndef FT_SSL_H
# define FT_SSL_H

# include <stddef.h>
# include <stdint.h>

typedef enum e_command_group
{
	COMMAND_GROUP_STANDARD,
	COMMAND_GROUP_MESSAGE_DIGEST,
	COMMAND_GROUP_CIPHER
} 	t_command_group;

struct s_command;
typedef int	(*t_command_handler)(const struct s_command *command,
				int argc, char **argv);

typedef struct s_command
{
	const char			*name;
	t_command_group		group;
	t_command_handler	handler;
	const void			*data;
} 	t_command;

typedef struct s_buffer
{
	unsigned char	*data;
	size_t			len;
	size_t			capacity;
} 	t_buffer;

typedef struct s_digest_algorithm
{
	const char	*name;
	const char	*display_name;
	size_t		digest_size;
	void		(*compute)(const unsigned char *data, size_t len,
					unsigned char *digest);
} 	t_digest_algorithm;

typedef struct s_digest_options
{
	int	quiet;
	int	reverse;
} 	t_digest_options;

typedef enum e_digest_source_kind
{
	DIGEST_SOURCE_STDIN_DEFAULT,
	DIGEST_SOURCE_STDIN_P,
	DIGEST_SOURCE_STRING,
	DIGEST_SOURCE_FILE
} 	t_digest_source_kind;

int					ft_is_streq(const char *a, const char *b);
size_t				ft_strlen(const char *str);
char				*ft_strdup(const char *src);
int					ft_strcmp(const char *a, const char *b);
void				ft_memcpy(void *dst, const void *src, size_t len);
void				ft_memset(void *dst, int c, size_t len);

int					buffer_append(t_buffer *buffer,
						const unsigned char *data, size_t len);
void				buffer_free(t_buffer *buffer);
int					read_stream(int fd, t_buffer *buffer);
int					read_file(const char *path, t_buffer *buffer);
int					write_all(int fd, const void *data, size_t len);

void				ft_putstr_fd(int fd, const char *str);
void				ft_putendl_fd(int fd, const char *str);
void				ft_puthex_fd(int fd, const unsigned char *data, size_t len);
void				print_invalid_command(const char *name);
void				print_system_error(const char *command,
						const char *target);
void				print_digest_error(const char *algorithm,
						const char *target);
void				print_command_list(void);
void				print_usage(void);

const t_command		*find_command(const char *name);
const t_command		*get_command_table(size_t *count);

int					run_help_command(const t_command *command,
						int argc, char **argv);
int					run_digest_command(const t_command *command,
						int argc, char **argv);

const t_digest_algorithm	*get_md5_algorithm(void);
const t_digest_algorithm	*get_sha256_algorithm(void);
void						format_digest_output(
							const t_digest_algorithm *algorithm,
							const t_digest_options *options,
							t_digest_source_kind source_kind,
							const char *label,
							size_t label_len,
							const unsigned char *digest);

#endif
