#include <stdarg.h>
#include "../../include/env_state.h"
#include "../../include/parse_tree.h"
#include <unistd.h>
#include <fcntl.h>
#include "executor_private.h"

int	set_input_fd(t_cmd *cmd, t_env *env, int custom_in)
{
	int		in_file;
	int		heredoc_pipe[2];

	if (cmd->in_filename)
	{
		in_file = open(cmd->in_filename, O_RDONLY);
		if (in_file == -1)
			return (report_func_error(env, "open"));
		dup2(in_file, STDIN_FILENO);
	}
	else if (cmd->heredocs_top > -1)
	{
		if (pipe(heredoc_pipe) == -1)
			return (report_func_error(env, "pipe"));
		read_heredoc(cmd, env, heredoc_pipe[1]);
		dup2(heredoc_pipe[0], STDIN_FILENO);
	}
	else
		dup2(custom_in, STDIN_FILENO);
	return (1);
}

int	set_output_fd(t_cmd *cmd, t_env *env, int custom_out)
{
	int	out_file;

	if (cmd->out_filename)
	{
		if (cmd->append_mode)
			out_file = open(cmd->out_filename,
					O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			out_file = open(cmd->out_filename, O_CREAT | O_RDWR | O_TRUNC,
					0644);
		if (out_file == -1)
			return (report_func_error(env, "open"));
		dup2(out_file, STDOUT_FILENO);
	}
	else
		dup2(custom_out, STDOUT_FILENO);
	return (1);
}

void	close_descriptors(int num_fd, ...)
{
	va_list	fds;
	int		i;
	int		curr_fd;

	i = -1;
	va_start(fds, num_fd);
	while (++i < num_fd)
	{
		curr_fd = va_arg(fds, int);
		close(curr_fd);
	}
	va_end(fds);
}
