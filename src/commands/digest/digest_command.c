#include "ft_ssl.h"
#include <unistd.h>

typedef struct s_digest_runtime
{
	const t_digest_algorithm	*algorithm;
	t_digest_options			options;
	int						parsed_operand;
	int						processed_any;
	int						had_error;
	int						stdin_loaded;
	t_buffer					stdin_buffer;
} 	t_digest_runtime;

static const t_digest_algorithm	*resolve_algorithm(const char *name);

static size_t	trimmed_label_len(const t_buffer *buffer)
{
	size_t	len;

	len = buffer->len;
	if (len > 0 && buffer->data[len - 1] == '\n')
		len--;
	return (len);
}

static int	load_stdin_once(t_digest_runtime *runtime)
{
	if (!runtime->stdin_loaded)
	{
		if (!read_stream(STDIN_FILENO, &runtime->stdin_buffer))
			return (0);
		runtime->stdin_loaded = 1;
	}
	return (1);
}

static void	compute_and_print(const t_digest_runtime *runtime,
	t_digest_source_kind source_kind, const char *label, size_t label_len,
	const t_buffer *buffer)
{
	unsigned char	digest[32];

	runtime->algorithm->compute(buffer->data, buffer->len, digest);
	format_digest_output(runtime->algorithm, &runtime->options, source_kind,
		label, label_len, digest);
}

static int	process_stdin_p(t_digest_runtime *runtime)
{
	size_t	label_len;

	if (!load_stdin_once(runtime))
		return (0);
	if (runtime->options.quiet)
		write_all(STDOUT_FILENO, runtime->stdin_buffer.data, runtime->stdin_buffer.len);
	label_len = trimmed_label_len(&runtime->stdin_buffer);
	compute_and_print(runtime, DIGEST_SOURCE_STDIN_P,
		(const char *)runtime->stdin_buffer.data, label_len,
		&runtime->stdin_buffer);
	runtime->processed_any = 1;
	return (1);
}

static void	process_string(t_digest_runtime *runtime, const char *value)
{
	t_buffer	buffer;

	buffer.data = (unsigned char *)value;
	buffer.len = ft_strlen(value);
	buffer.capacity = buffer.len;
	compute_and_print(runtime, DIGEST_SOURCE_STRING, value, buffer.len, &buffer);
	runtime->processed_any = 1;
}

static void	process_file(t_digest_runtime *runtime, const char *path)
{
	t_buffer	buffer;

	buffer.data = NULL;
	buffer.len = 0;
	buffer.capacity = 0;
	if (!read_file(path, &buffer))
	{
		print_digest_error(runtime->algorithm->name, path);
		runtime->had_error = 1;
		return ;
	}
	compute_and_print(runtime, DIGEST_SOURCE_FILE, path, ft_strlen(path), &buffer);
	buffer_free(&buffer);
	runtime->processed_any = 1;
}

static int	process_default_stdin(t_digest_runtime *runtime)
{
	if (!load_stdin_once(runtime))
		return (0);
	compute_and_print(runtime, DIGEST_SOURCE_STDIN_DEFAULT, NULL, 0,
		&runtime->stdin_buffer);
	runtime->processed_any = 1;
	return (1);
}

static int	handle_option(t_digest_runtime *runtime, int argc, char **argv, int *index)
{
	if (ft_is_streq(argv[*index], "-p"))
		return (process_stdin_p(runtime));
	if (ft_is_streq(argv[*index], "-q"))
	{
		runtime->options.quiet = 1;
		return (1);
	}
	if (ft_is_streq(argv[*index], "-r"))
	{
		runtime->options.reverse = 1;
		return (1);
	}
	if (ft_is_streq(argv[*index], "-s"))
	{
		if (*index + 1 >= argc)
		{
			ft_putstr_fd(2, "ft_ssl: ");
			ft_putstr_fd(2, runtime->algorithm->name);
			ft_putendl_fd(2, ": option requires an argument -- s");
			runtime->had_error = 1;
			return (0);
		}
		*index += 1;
		process_string(runtime, argv[*index]);
		return (1);
	}
	runtime->parsed_operand = 1;
	process_file(runtime, argv[*index]);
	return (1);
}

int	run_digest_command(const t_command *command, int argc, char **argv)
{
	t_digest_runtime	runtime;
	int					index;
	int					status;

	runtime.algorithm = resolve_algorithm(command->name);
	runtime.options.quiet = 0;
	runtime.options.reverse = 0;
	runtime.parsed_operand = 0;
	runtime.processed_any = 0;
	runtime.had_error = 0;
	runtime.stdin_loaded = 0;
	runtime.stdin_buffer.data = NULL;
	runtime.stdin_buffer.len = 0;
	runtime.stdin_buffer.capacity = 0;
	index = 0;
	status = 1;
	while (index < argc && status)
	{
		if (!runtime.parsed_operand)
			status = handle_option(&runtime, argc, argv, &index);
		else
			process_file(&runtime, argv[index]);
		index++;
	}
	if (status && !runtime.processed_any)
		status = process_default_stdin(&runtime);
	buffer_free(&runtime.stdin_buffer);
	if (!status || runtime.had_error)
		return (1);
	return (0);
}

static void	write_label(const char *label, size_t label_len)
{
	if (label != NULL && label_len > 0)
		write_all(STDOUT_FILENO, label, label_len);
}

void	format_digest_output(const t_digest_algorithm *algorithm,
	const t_digest_options *options, t_digest_source_kind source_kind,
	const char *label, size_t label_len, const unsigned char *digest)
{
	if (options->quiet)
	{
		ft_puthex_fd(STDOUT_FILENO, digest, algorithm->digest_size);
		write_all(STDOUT_FILENO, "\n", 1);
		return ;
	}
	if (options->reverse && source_kind != DIGEST_SOURCE_STDIN_DEFAULT
		&& source_kind != DIGEST_SOURCE_STDIN_P)
	{
		ft_puthex_fd(STDOUT_FILENO, digest, algorithm->digest_size);
		write_all(STDOUT_FILENO, " ", 1);
		if (source_kind == DIGEST_SOURCE_STRING)
			write_all(STDOUT_FILENO, "\"", 1);
		write_label(label, label_len);
		if (source_kind == DIGEST_SOURCE_STRING)
			write_all(STDOUT_FILENO, "\"", 1);
		write_all(STDOUT_FILENO, "\n", 1);
		return ;
	}
	if (source_kind == DIGEST_SOURCE_STDIN_DEFAULT)
		ft_putstr_fd(STDOUT_FILENO, "(stdin)= ");
	else if (source_kind == DIGEST_SOURCE_STDIN_P)
	{
		write_all(STDOUT_FILENO, "(\"", 2);
		write_label(label, label_len);
		ft_putstr_fd(STDOUT_FILENO, "\")= ");
	}
	else
	{
		ft_putstr_fd(STDOUT_FILENO, algorithm->display_name);
		write_all(STDOUT_FILENO, " (", 2);
		if (source_kind == DIGEST_SOURCE_STRING)
			write_all(STDOUT_FILENO, "\"", 1);
		write_label(label, label_len);
		if (source_kind == DIGEST_SOURCE_STRING)
			write_all(STDOUT_FILENO, "\"", 1);
		ft_putstr_fd(STDOUT_FILENO, ") = ");
	}
	ft_puthex_fd(STDOUT_FILENO, digest, algorithm->digest_size);
	write_all(STDOUT_FILENO, "\n", 1);
}

static const t_digest_algorithm	*resolve_algorithm(const char *name)
{
	if (ft_is_streq(name, "md5"))
		return (get_md5_algorithm());
	return (get_sha256_algorithm());
}
