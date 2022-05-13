/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:30:23 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:34:59 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"
#include <stdio.h>
#include "../../include/env_state.h"
#include "../../include/error.h"
#include "../../include/parse_tree.h"
#include "../../include/env_vars.h"
#include "../../include/free.h"
#include <readline/history.h>
#include <sys/param.h>

static int	check_cd(t_cmd *cmd, t_env *env, char *dir_path)
{
	if (cmd->argv_top > 1)
	{
		env->error_custom_msg = CD_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return (1);
	}
	if (cmd->argv_top == 0)
	{
		dir_path = get("HOME", env->global_env_vars);
		if (!dir_path)
		{
			env->error_custom_msg = "HOME path is not set";
			env->exit_code = EXIT_FAILURE;
			return (1);
		}
	}
	return (0);
}

static int	check_other(char *dir_path, t_env *env, char *buf)
{
	if (chdir(dir_path) == -1)
	{
		env->error_func_name = "chdir";
		env->exit_code = EXIT_FAILURE;
		return (1);
	}
	if (getcwd(buf, MAXPATHLEN) == NULL)
	{
		env->error_func_name = "getcwd";
		env->exit_code = EXIT_FAILURE;
		return (1);
	}
	return (0);
}

static int	check_pwd(char *pwd_value, t_env *env)
{
	if (!pwd_value)
	{
		env->error_func_name = "malloc";
		env->exit_code = EXIT_FAILURE;
		return (1);
	}
	return (0);
}

void	cd(t_cmd *cmd, t_env *env)
{
	char	*pwd_value;
	char	*dir_path;
	char	buf[MAXPATHLEN];

	dir_path = NULL;
	if (check_cd(cmd, env, dir_path))
		return ;
	else
		dir_path = cmd->argv[1];
	if (check_other(dir_path, env, buf))
		return ;
	pwd_value = ft_strjoin("PWD=", buf);
	if (check_pwd(pwd_value, env))
		return ;
	set("PWD", pwd_value, &(env->global_env_vars), env);
	if (env->error_func_name)
	{
		free(pwd_value);
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	env->exit_code = EXIT_SUCCESS;
}

void	pwd(t_cmd *cmd, t_env *env)
{
	char	buf[MAXPATHLEN];

	if (cmd->argv_top != 0)
	{
		env->error_custom_msg = PWD_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	if (getcwd(buf, MAXPATHLEN) == NULL)
	{
		env->error_func_name = "getcwd";
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	ft_putstr_fd(buf, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	env->exit_code = EXIT_SUCCESS;
}
