#include "executor.h"

void	free_2d_arr(void **arr)
{
	int	i;

	i = -1;
	while (arr[++i] != NULL)
		free(arr[i]);
	free(arr);
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

void	find_and_exec_cmd(t_cmd *cmd)
{
	char	**paths;
	int		path_idx;
	int		i;

	path_idx = find_path_idx(environ);
	paths = ft_split(environ[path_idx] + 5, ':');
	i = -1;
	while (paths[++i])
		search_bin(paths, cmd, environ, i);
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
					O_CREAT | O_RDWR | O_APPEND);
		else
			out_file = open(cmd->out_filename, O_CREAT | O_RDWR | O_TRUNC);
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
			close(second_p[0]);
		if (curr_pipelist->type == NEXT_PIPELST)
		{
			set_input_fd(curr_pipelist->u_item.cmd, global_in);
			if (curr_pipelist->next != NULL)
				set_output_fd(curr_pipelist->u_item.cmd, second_p[1]);
			else
				set_output_fd(curr_pipelist->u_item.cmd, global_out);
			find_and_exec_cmd(curr_pipelist->u_item.cmd);
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
		close(second_p[0]);
		close(second_p[1]);
		pipe(second_p);
		curr_process = fork();
		if (!curr_process)
		{
			close(first_p[1]);
			close(second_p[0]);
			if (curr_pipelist->type == NEXT_PIPELST)
			{
				set_input_fd(curr_pipelist->u_item.cmd, first_p[0]);
				set_output_fd(curr_pipelist->u_item.cmd, second_p[1]);
				find_and_exec_cmd(curr_pipelist->u_item.cmd);
			}
			else if (curr_pipelist->type == NEXT_SCRIPT)
				executor(curr_pipelist->u_item.script, env, first_p[0],
					second_p[1]);
		}
		close(first_p[0]);
		close(first_p[1]);
		curr_pipelist = curr_pipelist->next;
	}
	curr_process = fork();
	if (!curr_process)
	{
		close(second_p[1]);
		if (curr_pipelist->type == NEXT_PIPELST)
		{
			set_input_fd(curr_pipelist->u_item.cmd, second_p[0]);
			set_output_fd(curr_pipelist->u_item.cmd, global_out);
			find_and_exec_cmd(curr_pipelist->u_item.cmd);
		}
		else if (curr_pipelist->type == NEXT_SCRIPT)
			executor(curr_pipelist->u_item.script, env,
				second_p[0], global_out);
	}
	close(second_p[1]);
	close(second_p[0]);
	while (wait(&status) > 0)
		;
	env->exit_code = WEXITSTATUS(status);
}

void	iter_cmd_table(t_cmd_table *curr_cmd_table, t_env *env,
						int global_in, int global_out)
{
	int	i;

	i = 0;
	while (i <= curr_cmd_table->top)
	{
		if (curr_cmd_table->logical_op[i] == OR)
		{
			if (env->exit_code == EXIT_SUCCESS)
				i++;
			else
				iter_pipelist(curr_cmd_table->cmd_arr[i++], env,
					global_in, global_out);
		}
		else if (curr_cmd_table->logical_op[i] == AND)
		{
			if (env->exit_code != EXIT_SUCCESS)
				i++;
			else
				iter_pipelist(curr_cmd_table->cmd_arr[i++], env,
					global_in, global_out);
		}
		else
			iter_pipelist(curr_cmd_table->cmd_arr[i++], env,
				global_in, global_out);
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