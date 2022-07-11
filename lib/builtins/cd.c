/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:12:26 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:12:28 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins_private.h"

static int	check_args_home(t_cmd *cmd, char **dir_path)
{
	if (cmd->argv_top > 1)
	{
		g_env->error_custom_msg = CD_ARGC_ERROR;
		g_env->exit_code = EXIT_FAILURE;
		return (0);
	}
	if (cmd->argv_top == 0)
	{
		*dir_path = get("HOME", g_env->global_env_vars);
		if (!*dir_path)
		{
			g_env->error_custom_msg = "HOME path is not set";
			g_env->exit_code = EXIT_FAILURE;
			return (0);
		}
	}
	else
		*dir_path = cmd->argv[1];
	return (1);
}

static int	set_pwd(void)
{
	char	buf[MAXPATHLEN];
	char	*pwd_value;

	if (getcwd(buf, MAXPATHLEN) == NULL)
	{
		g_env->error_func_name = "getcwd";
		g_env->exit_code = EXIT_FAILURE;
		return (0);
	}
	pwd_value = ft_strjoin("PWD=", buf);
	if (!pwd_value)
	{
		g_env->error_func_name = "malloc";
		g_env->exit_code = EXIT_FAILURE;
		return (0);
	}
	set("PWD", pwd_value, &(g_env->global_env_vars), g_env);
	if (g_env->error_func_name)
	{
		free(pwd_value);
		g_env->exit_code = EXIT_FAILURE;
		return (0);
	}
	return (1);
}

void	cd(t_cmd *cmd, t_env *env)
{
	char	*dir_path;

	if (!check_args_home(cmd, &dir_path))
		return ;
	if (chdir(dir_path) == -1)
	{
		env->error_func_name = "chdir";
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	if (!set_pwd())
		return ;
	env->exit_code = EXIT_SUCCESS;
}
