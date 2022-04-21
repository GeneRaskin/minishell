#include "../../include/executor.h"
#include "../../include/tokens.h"
#include "../../include/free.h"
#include "../../include/parse_tree.h"
#include "../../include/env_state.h"
#include "../../include/env_vars.h"
#include "../../include/libft.h"
#include "../../include/error.h"
#include "../../include/builtins.h"
#include <sys/wait.h>
#include <stdarg.h>
#include <fcntl.h>

void	read_heredoc(t_cmd *cmd, int write_pipe);

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

static char	**construct_envp(t_env *env)
{
	t_env_vars	*globals;
	char		**envp;
	int			len;
	char		*value;
	char		*temp;

	globals = env->global_env_vars;
	len = 0;
	while (globals != NULL)
	{
		len++;
		globals = globals->next;
	}
	envp = (char **) malloc(sizeof(char *) * (len + 1));
	len = 0;
	globals = env->global_env_vars;
	while (globals != NULL)
	{
		value = ft_strjoin(globals->name, "=");
		temp = value;
		value = ft_strjoin(value, globals->value);
		free(temp);
		globals = globals->next;
		envp[len++] = value;
	}
	envp[len + 1] = NULL;
	return (envp);
}

static void	search_bin(char **paths, t_cmd *cmd, t_env *env, int i)
{
	char	*path_with_slash;
	char	*curr_bin;
	char	**envp;

	path_with_slash = ft_strjoin(paths[i], "/");
	curr_bin = ft_strjoin(path_with_slash, cmd->argv[0]);
	free(path_with_slash);
	if (access(curr_bin, F_OK) == 0)
	{
		envp = construct_envp(env);
		execve(curr_bin, cmd->argv, envp);
	}
	free(curr_bin);
}

void	find_and_exec_cmd(t_cmd *cmd, t_env *env)
{
	int		i;
	char	**paths;
	char	**envp;

	if (cmd->argv_top == -1)
		return ;
	i = 0;
	paths = ft_split(getenv("PATH"), ':');
	while (paths[++i])
		search_bin(paths, cmd, env, i);
	if (paths[i] == NULL)
	{
		if (access(cmd->argv[0], F_OK) == 0)
		{
			envp = construct_envp(env);
			execve(cmd->argv[0], cmd->argv, envp);
		}
		env->error_custom_msg = BIN_NOT_FOUND_ERR;
		error(env);
		exit(EXIT_SUCCESS);
	}
	free_2d_arr((void **)paths);
}

void	set_input_fd(t_cmd *cmd, t_env *env, int custom_in)
{
	int		in_file;
	int		heredoc_pipe[2];

	if (cmd->in_filename)
	{
		in_file = open(cmd->in_filename, O_RDONLY);
		dup2(in_file, STDIN_FILENO);
	}
	else if (cmd->heredocs_top > -1)
	{
		if (pipe(heredoc_pipe) == -1)
			env->error_func_name = "pipe";
		read_heredoc(cmd, heredoc_pipe[1]);
		dup2(heredoc_pipe[0], STDIN_FILENO);
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
	int		standard_backup[2];
	int		status;
	pid_t	curr_process;

	if (curr_pipelist->type == NEXT_PIPELST
		&& curr_pipelist->u_item.cmd->argv_top == -1)
		return ;
	if (curr_pipelist->next != NULL)
		if (pipe(second_p) == -1)
			env->error_func_name = "pipe";
	if (curr_pipelist->type == NEXT_PIPELST && find_builtin(
			curr_pipelist->u_item.cmd, &status))
	{
		standard_backup[0] = dup(STDIN_FILENO);
		standard_backup[1] = dup(STDOUT_FILENO);
		set_input_fd(curr_pipelist->u_item.cmd, env, global_in);
		if (curr_pipelist->next != NULL)
			set_output_fd(curr_pipelist->u_item.cmd, second_p[1]);
		else
			set_output_fd(curr_pipelist->u_item.cmd, global_out);
		call_builtins(curr_pipelist->u_item.cmd, env, status);
		dup2(standard_backup[0], STDIN_FILENO);
		dup2(standard_backup[1], STDOUT_FILENO);
		close_descriptors(2, standard_backup[0], standard_backup[1]);
		if (curr_pipelist->next == NULL)
			return ;
	}
	else
	{
		curr_process = fork();
		if (!curr_process)
		{
			if (curr_pipelist->next != NULL)
				close_descriptors(1, second_p[0]);
			if (curr_pipelist->type == NEXT_PIPELST)
			{
				set_input_fd(curr_pipelist->u_item.cmd, env, global_in);
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
			exit(EXIT_SUCCESS);
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
	}
	while (curr_pipelist->next != NULL)
	{
		first_p[0] = dup(second_p[0]);
		first_p[1] = dup(second_p[1]);
		close_descriptors(2, second_p[0], second_p[1]);
		if (pipe(second_p) == -1)
			env->error_func_name = "pipe";
		if (curr_pipelist->type == NEXT_PIPELST && find_builtin(
				curr_pipelist->u_item.cmd, &status))
		{
			standard_backup[0] = dup(STDIN_FILENO);
			standard_backup[1] = dup(STDOUT_FILENO);
			set_input_fd(curr_pipelist->u_item.cmd, env, first_p[0]);
			set_output_fd(curr_pipelist->u_item.cmd, second_p[1]);
			call_builtins(curr_pipelist->u_item.cmd, env, status);
			dup2(standard_backup[0], STDIN_FILENO);
			dup2(standard_backup[1], STDOUT_FILENO);
			close_descriptors(2, standard_backup[0], standard_backup[1]);
		}
		else
		{
			curr_process = fork();
			if (!curr_process)
			{
				close_descriptors(2, first_p[1], second_p[0]);
				if (curr_pipelist->type == NEXT_PIPELST)
				{
					set_input_fd(curr_pipelist->u_item.cmd, env, first_p[0]);
					set_output_fd(curr_pipelist->u_item.cmd, second_p[1]);
					find_and_exec_cmd(curr_pipelist->u_item.cmd, env);
				}
				else if (curr_pipelist->type == NEXT_SCRIPT)
					executor(curr_pipelist->u_item.script, env, first_p[0],
						second_p[1]);
				exit(EXIT_SUCCESS);
			}
		}
		close_descriptors(2, first_p[0], first_p[1]);
		curr_pipelist = curr_pipelist->next;
	}
	if (curr_pipelist->type == NEXT_PIPELST && find_builtin(
			curr_pipelist->u_item.cmd, &status))
	{
		standard_backup[0] = dup(STDIN_FILENO);
		standard_backup[1] = dup(STDOUT_FILENO);
		set_input_fd(curr_pipelist->u_item.cmd, env, second_p[0]);
		set_output_fd(curr_pipelist->u_item.cmd, global_out);
		call_builtins(curr_pipelist->u_item.cmd, env, status);
		dup2(standard_backup[0], STDIN_FILENO);
		dup2(standard_backup[1], STDOUT_FILENO);
		close_descriptors(2, standard_backup[0], standard_backup[1]);
	}
	else
	{
		curr_process = fork();
		if (!curr_process)
		{
			close_descriptors(1, second_p[1]);
			if (curr_pipelist->type == NEXT_PIPELST)
			{
				set_input_fd(curr_pipelist->u_item.cmd, env, second_p[0]);
				set_output_fd(curr_pipelist->u_item.cmd, global_out);
				find_and_exec_cmd(curr_pipelist->u_item.cmd, env);
			}
			else if (curr_pipelist->type == NEXT_SCRIPT)
				executor(curr_pipelist->u_item.script, env,
					second_p[0], global_out);
			exit(EXIT_SUCCESS);
		}
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
}
