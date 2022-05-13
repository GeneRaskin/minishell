/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_exec_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 23:12:19 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/13 23:12:21 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env_state.h"
#include "../../include/parse_tree.h"
#include "../../include/libft.h"
#include <sys/param.h>
#include "../../include/error.h"
#include "../../include/free.h"
#include "executor_private.h"
#include "../../include/env_vars.h"
#include <stdio.h>
#include <readline/history.h>

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
	if (envp == NULL)
	{
		report_func_error(env, "malloc");
		return (NULL);
	}
	len = 0;
	globals = env->global_env_vars;
	while (globals != NULL)
	{
		value = ft_strjoin(globals->name, "=");
		if (value == NULL)
		{
			report_func_error(env, "malloc");
			globals = globals->next;
			continue ;
		}
		temp = value;
		value = ft_strjoin(value, globals->value);
		if (value == NULL)
		{
			report_func_error(env, "malloc");
			free(temp);
			globals = globals->next;
			continue ;
		}
		free(temp);
		globals = globals->next;
		envp[len++] = value;
	}
	envp[len] = NULL;
	return (envp);
}

static void	terminate(char **paths, t_env *env)
{
	if (paths)
		free_2d_arr((void **)paths);
	free_env_vars(env->env_vars);
	free_env_vars(env->global_env_vars);
	free_parse_tree(env->parse_tree);
	clear_history();
	exit(EXIT_FAILURE);
}

static void	search_bin(char **paths, t_cmd *cmd, t_env *env, int i)
{
	char	*path_with_slash;
	char	*curr_bin;
	char	**envp;

	path_with_slash = ft_strjoin(paths[i], "/");
	if (path_with_slash == NULL)
	{
		report_func_error(env, "malloc");
		terminate(paths, env);
	}
	curr_bin = ft_strjoin(path_with_slash, cmd->argv[0]);
	if (curr_bin == NULL)
	{
		free(path_with_slash);
		report_func_error(env, "malloc");
		terminate(paths, env);
	}
	free(path_with_slash);
	if (access(curr_bin, F_OK) == 0)
	{
		envp = construct_envp(env);
		if (envp == NULL)
		{
			free(curr_bin);
			terminate(paths, env);
		}
		if (execve(curr_bin, cmd->argv, envp) == -1)
		{
			free(curr_bin);
			report_func_error(env, "execve");
			terminate(paths, env);
		}
	}
	free(curr_bin);
}

static char	**iter_paths(t_cmd *cmd, t_env *env, int *i,
						char **path_with_slash)
{
	char	**paths;

	*i = 0;
	path_with_slash[0] = getenv("PATH");
	if (path_with_slash[0] == NULL)
	{
		report_func_error(env, "getenv");
		terminate(NULL, env);
	}
	paths = ft_split(path_with_slash[0], ':');
	if (paths == NULL)
	{
		report_func_error(env, "malloc");
		terminate(NULL, env);
	}
	while (paths[++(*i)])
		search_bin(paths, cmd, env, *i);
	return (paths);
}

void	find_and_exec_cmd(t_cmd *cmd, t_env *env)
{
	int		i;
	char	**paths;
	char	**envp;
	char	*path_with_slash[2];
	char	curr_path[MAXPATHLEN];

	if (cmd->argv_top == -1)
		return ;
	paths = iter_paths(cmd, env, &i, path_with_slash);
	if (paths[i] == NULL)
	{
		if (cmd->argv[0][0] != '/')
		{
			path_with_slash[0] = ft_strjoin(getcwd(curr_path, MAXPATHLEN), "/");
			if (path_with_slash[0] == NULL)
			{
				report_func_error(env, "malloc");
				terminate(paths, env);
			}
			path_with_slash[1] = ft_strjoin(path_with_slash[0], cmd->argv[0]);
			if (path_with_slash[1] == NULL)
			{
				free(path_with_slash[0]);
				report_func_error(env, "malloc");
				terminate(NULL, env);
			}
			free(path_with_slash[0]);
		}
		else
			path_with_slash[1] = cmd->argv[0];
		if (access(path_with_slash[1], F_OK) == 0)
		{
			envp = construct_envp(env);
			if (envp == NULL)
			{
				free(path_with_slash[1]);
				terminate(paths, env);
			}
			if (execve(path_with_slash[1], cmd->argv, envp) == -1)
			{
				report_func_error(env, "execve");
				free(path_with_slash[1]);
				terminate(paths, env);
			}
		}
		if (cmd->argv[0][0] != '/')
			free(path_with_slash[1]);
		set_err_custom_msg(env, BIN_NOT_FOUND_ERR);
		error(env);
	}
	terminate(paths, env);
}
