/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:43 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:43 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtins.h"
#include "../../include/libft.h"
#include "../../include/env_state.h"
#include "../../include/parse_tree.h"
#include "../../include/error.h"

void	call_builtins(t_cmd *cmd, t_env *env, int which_builtin)
{
	if (which_builtin == CD)
		cd(cmd, env);
	else if (which_builtin == ENV)
		ft_env(cmd, env);
	else if (which_builtin == PWD)
		pwd(cmd, env);
	else if (which_builtin == EXIT)
		ft_exit(cmd, env);
	else if (which_builtin == UNSET)
		ft_unset(cmd, env);
	else if (which_builtin == ECHO)
		echo(cmd, env);
	else if (which_builtin == EXPORT)
		export(cmd, env);
	if (env->error_custom_msg || env->error_func_name)
		error(env);
}

int	find_builtin(t_cmd *cmd, int *which_builtin)
{
	size_t	len;
	char	*name;

	name = cmd->argv[0];
	len = ft_strlen(name);
	if (!ft_strncmp("cd", name, len) && len == 2)
		*which_builtin = CD;
	else if (!ft_strncmp("pwd", name, len) && len == 3)
		*which_builtin = PWD;
	else if (!ft_strncmp("env", name, len) && len == 3)
		*which_builtin = ENV;
	else if (!ft_strncmp("exit", name, len) && len == 4)
		*which_builtin = EXIT;
	else if (!ft_strncmp("unset", name, len) && len == 5)
		*which_builtin = UNSET;
	else if (!ft_strncmp("export", name, len) && len == 6)
		*which_builtin = EXPORT;
	else if (!ft_strncmp("echo", name, len) && len == 4)
		*which_builtin = ECHO;
	else
		return (0);
	return (1);
}
