#include "../../include/executor.h"
#include "../../include/tokens.h"
#include "../../include/free.h"
#include "../../include/parse_tree.h"
#include "../../include/env_state.h"
#include "../../include/libft.h"
#include "../../include/error.h"
#include <stdarg.h>
#include <fcntl.h>

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

static int	find_path_idx(char **envp)
{
	int	path_idx;

	path_idx = -1;
	while (envp[++path_idx])
		if (ft_strncmp(envp[path_idx], "PATH=", 5) == 0)
			return (path_idx);
	return (-1);
}

static void	search_bin(char **paths, t_cmd *cmd, char **envp, int i)
{
	char	*path_with_slash;
	char	*curr_bin;

	path_with_slash = ft_strjoin(paths[i], "/");
	curr_bin = ft_strjoin(path_with_slash, cmd->argv[0]);
	free(path_with_slash);
	if (access(curr_bin, F_OK) == 0)
		execve(curr_bin, cmd->argv, envp);
	free(curr_bin);
}

void	find_and_exec_cmd(t_cmd *cmd, t_env *env)
{
	char	**paths;
	int		path_idx;
	int		i;

	if (cmd->argv_top == -1)
		return ;
	path_idx = find_path_idx(environ);
	paths = ft_split(environ[path_idx] + 5, ':');
	i = -1;
	while (paths[++i])
		search_bin(paths, cmd, environ, i);
	if (paths[i] == NULL)
	{
		env->error_custom_msg = BIN_NOT_FOUND_ERR;
		error(env);
	}
	free_2d_arr((void **)paths);
}

void	set_input_fd(t_cmd *cmd, int custom_in)
{
	int	in_file;

	if (cmd->in_filename)
	{
		in_file = open(cmd->in_filename, O_RDONLY);
		dup2(in_file, STDIN_FILENO);
	}
	else
		dup2(custom_in, STDIN_FILENO);
}

void	set_output_fd(t_cmd *cmd, int custom_out)
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
		dup2(out_file, STDOUT_FILENO);
	}
	else
		dup2(custom_out, STDOUT_FILENO);
}

void	iter_pipelist(t_pipelist *curr_pipelist, t_env *env,
					  int global_in, int global_out)
{
	int		first_p[2];
	int		second_p[2];
	int		status;
	pid_t	curr_process;

	if (curr_pipelist->next != NULL)
		pipe(second_p);
	curr_process = fork();
	if (!curr_process)
	{
		if (curr_pipelist->next != NULL)
			close_descriptors(1, second_p[0]);
		if (curr_pipelist->type == NEXT_PIPELST)
		{
			set_input_fd(curr_pipelist->u_item.cmd, global_in);
			if (curr_pipelist->next != NULL)
				set_output_fd(curr_pipelist->u_item.cmd, second_p[1]);
			else
				set_output_fd(curr_pipelist->u_item.cmd, global_out);
			find_and_exec_cmd(curr_pipelist->u_item.cmd, env);
		}
		else if (curr_pipelist->type == NEXT_SCRIPT)
		{
			if (curr_pipelist->next != NULL)
				executor(curr_pipelist->u_item.script, env, global_in,
					second_p[1]);
			else
				executor(curr_pipelist->u_item.script, env, global_in,
					global_out);
		}
	}
	if (curr_pipelist->next != NULL)
		curr_pipelist = curr_pipelist->next;
	else
	{
		while (wait(&status) > 0)
			;
		env->exit_code = WEXITSTATUS(status);
		return ;
	}
	while (curr_pipelist->next != NULL)
	{
		first_p[0] = dup(second_p[0]);
		first_p[1] = dup(second_p[1]);
		close_descriptors(2, second_p[0], second_p[1]);
		pipe(second_p);
		curr_process = fork();
		if (!curr_process)
		{
			close_descriptors(2, first_p[1], second_p[0]);
			if (curr_pipelist->type == NEXT_PIPELST)
			{
				set_input_fd(curr_pipelist->u_item.cmd, first_p[0]);
				set_output_fd(curr_pipelist->u_item.cmd, second_p[1]);
				find_and_exec_cmd(curr_pipelist->u_item.cmd, env);
			}
			else if (curr_pipelist->type == NEXT_SCRIPT)
				executor(curr_pipelist->u_item.script, env, first_p[0],
					second_p[1]);
		}
		close_descriptors(2, first_p[0], first_p[1]);
		curr_pipelist = curr_pipelist->next;
	}
	curr_process = fork();
	if (!curr_process)
	{
		close_descriptors(1, second_p[1]);
		if (curr_pipelist->type == NEXT_PIPELST)
		{
			set_input_fd(curr_pipelist->u_item.cmd, second_p[0]);
			set_output_fd(curr_pipelist->u_item.cmd, global_out);
			find_and_exec_cmd(curr_pipelist->u_item.cmd, env);
		}
		else if (curr_pipelist->type == NEXT_SCRIPT)
			executor(curr_pipelist->u_item.script, env,
				second_p[0], global_out);
	}
	close_descriptors(2, second_p[1], second_p[0]);
	while (wait(&status) > 0)
		;
	env->exit_code = WEXITSTATUS(status);
}

void	iter_cmd_table(t_cmd_table *curr_cmd_table, t_env *env,
						int global_in, int global_out)
{
	while (curr_cmd_table != NULL)
	{
		if ((curr_cmd_table->logical_op == OR && env->exit_code == EXIT_SUCCESS)
			|| (curr_cmd_table->logical_op == AND
				&& env->exit_code != EXIT_SUCCESS))
			curr_cmd_table = curr_cmd_table->next;
		else
		{
			iter_pipelist(curr_cmd_table->pipelist, env,
				global_in, global_out);
			curr_cmd_table = curr_cmd_table->next;
		}
	}
}

void	executor(t_scripts *parse_tree, t_env *env,
					int global_in, int global_out)
{
	while (parse_tree != NULL)
	{
		iter_cmd_table(parse_tree->cmd_table, env, global_in, global_out);
		parse_tree = parse_tree->next;
	}
	exit(EXIT_SUCCESS);
}
