#include <stdarg.h>
#include "../../include/env_state.h"
#include "../../include/parse_tree.h"
#include <unistd.h>
#include <fcntl.h>
#include "executor_private.h"
#include <stdlib.h>

int	set_input_fd(t_cmd *cmd, t_env *env, int custom_in)
{
	int		in_file;
	int		i;
	int		heredoc_pipe[2];

	i = 0;
	if (cmd->heredocs_top != -1)
	{
		if (pipe(heredoc_pipe) == -1)
			return (report_func_error(env, "pipe"));
		read_heredoc(cmd, env, heredoc_pipe[1]);
		if (env->state & 0x400)
			return (0);
		if (cmd->last_input == 2)
			dup2(heredoc_pipe[0], STDIN_FILENO);
		else
			close_descriptors(2, heredoc_pipe[0], heredoc_pipe[1]);
	}
	while (i <= cmd->in_filenames_top)
	{
		in_file = open(cmd->in_filename[i], O_RDONLY);
		if (in_file == -1)
			return (report_func_error(env, "open"));
		if (cmd->last_input == 1 && i == cmd->in_filenames_top)
			dup2(in_file, STDIN_FILENO);
		else
			close_descriptors(1, in_file);
		i++;
	}
	if (cmd->last_input == 0)
		dup2(custom_in, STDIN_FILENO);
	return (1);
}

int	set_output_fd(t_cmd *cmd, t_env *env, int custom_out)
{
	int	out_file;
	int	i;

	i = 0;
	while (i <= cmd->out_filenames_top)
	{
		if (cmd->append_mode[i])
			out_file = open(cmd->out_filename[i],
					O_CREAT | O_RDWR | O_APPEND, 0644);
		else
			out_file = open(cmd->out_filename[i], O_CREAT | O_RDWR | O_TRUNC,
					0644);
		if (out_file == -1)
			return (report_func_error(env, "open"));
		if (i == cmd->out_filenames_top)
			dup2(out_file, STDOUT_FILENO);
		i++;
	}
	if (cmd->out_filenames_top == -1)
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
