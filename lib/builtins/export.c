/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:37:32 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:44:33 by lemmon           ###   ########.fr       */
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

void	ft_env(t_cmd *cmd, t_env *env);

static char	**get_vars_values(char *arg)
{
	char	**vars_values;
	int		var_len;
	int		val_len;

	var_len = 0;
	while (arg[var_len] && arg[var_len] != '=')
		var_len++;
	if (!arg[var_len])
		vars_values = (char **) malloc(sizeof(char *) * 2);
	else
		vars_values = (char **) malloc(sizeof(char *) * 3);
	if (!vars_values)
		return (NULL);
	vars_values[0] = ft_substr(arg, 0, var_len);
	if (!vars_values[0])
	{
		free(vars_values);
		return (NULL);
	}
	if (!arg[var_len])
		vars_values[1] = NULL;
	else
	{
		val_len = 0;
		while (arg[var_len + 1 + val_len])
			val_len++;
		vars_values[1] = ft_substr(arg, var_len + 1, val_len);
		if (!vars_values[1])
		{
			free(vars_values[0]);
			free(vars_values);
			return (NULL);
		}
		vars_values[2] = NULL;
	}
	return (vars_values);
}

void	export(t_cmd *cmd, t_env *env)
{
	int		i;
	char	**vars_values;
	char	*value;

	i = 1;
	if (cmd->argv_top == 0)
	{
		ft_env(cmd, env);
		return ;
	}
	else
	{
		while (i <= cmd->argv_top)
		{
			vars_values = get_vars_values(cmd->argv[i]);
			if (!vars_values)
			{
				env->error_func_name = "malloc";
				env->exit_code = EXIT_FAILURE;
				return ;
			}
			if (vars_values[1])
			{
				value = get(vars_values[0], env->global_env_vars);
				set(vars_values[0], vars_values[1],
					&(env->global_env_vars), env);
				if (env->error_func_name)
				{
					free(vars_values[1]);
					free(vars_values[0]);
					free(vars_values);
					env->exit_code = EXIT_FAILURE;
					return ;
				}
				if (get(vars_values[0], env->env_vars))
					unset(vars_values[0], &(env->env_vars));
				if (value)
					free(vars_values[0]);
				free(vars_values);
			}
			else
			{
				value = get(vars_values[0], env->env_vars);
				if (value)
				{
					value = ft_strdup(value);
					if (value == NULL)
					{
						free(vars_values[0]);
						free(vars_values);
						env->exit_code = EXIT_FAILURE;
						return ;
					}
					unset(vars_values[0], &(env->env_vars));
					set(vars_values[0], value, &(env->global_env_vars), env);
					if (env->error_func_name)
					{
						free(vars_values[0]);
						free(vars_values);
						free(value);
						env->exit_code = EXIT_FAILURE;
						return ;
					}
				}
				else
				{
					value = get(vars_values[0], env->global_env_vars);
					if (!value)
					{
						value = ft_strdup("");
						if (!value)
						{
							env->error_func_name = "malloc";
							env->exit_code = EXIT_FAILURE;
							free(vars_values[0]);
							free(vars_values);
							return ;
						}
						set(vars_values[0], value,
							&(env->global_env_vars), env);
						if (env->error_func_name)
						{
							free(value);
							free(vars_values[0]);
							free(vars_values);
							env->exit_code = EXIT_FAILURE;
							return ;
						}
					}
					else
						free(vars_values[0]);
				}
				free(vars_values);
			}
			i++;
		}
	}
	env->exit_code = EXIT_SUCCESS;
}
