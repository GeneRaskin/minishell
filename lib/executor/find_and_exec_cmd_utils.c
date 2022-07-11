/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_and_exec_cmd_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:35:07 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:21:47 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/env_state.h"
#include "../../include/env_vars.h"
#include "../../include/parse_tree.h"
#include "../../include/error.h"
#include "../../include/libft.h"
#include <sys/param.h>
#include "executor_private.h"

extern t_env	*g_env;

void	terminate(char **paths, t_env *env);
char	**construct_envp(t_env *env);

void	construct_bin(t_cmd *cmd, char **curr_bin, int i,
						char **paths)
{
	char	*path_with_slash;

	path_with_slash = ft_strjoin(paths[i], "/");
	if (path_with_slash == NULL)
	{
		report_func_error(g_env, "malloc");
		terminate(paths, g_env);
	}
	*curr_bin = ft_strjoin(path_with_slash, cmd->argv[0]);
	if (*curr_bin == NULL)
	{
		free(path_with_slash);
		report_func_error(g_env, "malloc");
		terminate(paths, g_env);
	}
	free(path_with_slash);
}

int	construct_envp_loop(t_env_vars	**globals, char **envp, int *len)
{
	char	*value;
	char	*temp;

	value = ft_strjoin((*globals)->name, "=");
	if (value == NULL)
	{
		report_func_error(g_env, "malloc");
		*globals = (*globals)->next;
		return (0);
	}
	temp = value;
	value = ft_strjoin(value, (*globals)->value);
	if (value == NULL)
	{
		report_func_error(g_env, "malloc");
		free(temp);
		*globals = (*globals)->next;
		return (0);
	}
	free(temp);
	*globals = (*globals)->next;
	envp[*len] = value;
	*len += 1;
	return (1);
}

void	rel_abs_paths_case(t_cmd *cmd, char *path_with_slash[2],
							char **paths)
{
	char	curr_path[MAXPATHLEN];

	if (cmd->argv[0][0] != '/')
	{
		path_with_slash[0] = ft_strjoin(getcwd(curr_path, MAXPATHLEN), "/");
		if (path_with_slash[0] == NULL)
		{
			report_func_error(g_env, "malloc");
			terminate(paths, g_env);
		}
		path_with_slash[1] = ft_strjoin(path_with_slash[0], cmd->argv[0]);
		if (path_with_slash[1] == NULL)
		{
			free(path_with_slash[0]);
			report_func_error(g_env, "malloc");
			terminate(NULL, g_env);
		}
		free(path_with_slash[0]);
	}
	else
		path_with_slash[1] = cmd->argv[0];
}

void	exec_rel_abs(t_cmd *cmd, char *path_with_slash[2],
						char **paths)
{
	char	**envp;

	if (access(path_with_slash[1], F_OK) == 0)
	{
		envp = construct_envp(g_env);
		if (envp == NULL)
		{
			free(path_with_slash[1]);
			terminate(paths, g_env);
		}
		if (execve(path_with_slash[1], cmd->argv, envp) == -1)
		{
			report_func_error(g_env, "execve");
			free(path_with_slash[1]);
			terminate(paths, g_env);
		}
	}
}
