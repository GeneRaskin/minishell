/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:12:40 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:19:13 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins_private.h"
#include "../../include/builtins.h"

extern t_env	*g_env;

static int	remainder_get_vars_values(char *arg, int var_len,
										char **vars_values)
{
	int	val_len;

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
			return (0);
		}
		vars_values[2] = NULL;
	}
	return (1);
}

static char	**get_vars_values(char *arg)
{
	char	**vars_values;
	int		var_len;

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
	if (!remainder_get_vars_values(arg, var_len, vars_values))
		return (NULL);
	return (vars_values);
}

static void	export_loop1(char **vars_values)
{
	char	*value;

	value = get(vars_values[0], g_env->global_env_vars);
	set(vars_values[0], vars_values[1],
		&(g_env->global_env_vars), g_env);
	if (get(vars_values[0], g_env->env_vars))
		unset(vars_values[0], &(g_env->env_vars));
	if (value)
		free(vars_values[0]);
	free(vars_values);
}

static void	export_loop2(char **vars_values)
{
	char	*value;

	value = get(vars_values[0], g_env->env_vars);
	if (value)
	{
		value = ft_strdup(value);
		unset(vars_values[0], &(g_env->env_vars));
		set(vars_values[0], value, &(g_env->global_env_vars), g_env);
	}
	else
	{
		value = get(vars_values[0], g_env->global_env_vars);
		if (!value)
		{
			value = ft_strdup("");
			set(vars_values[0], value,
				&(g_env->global_env_vars), g_env);
		}
		else
			free(vars_values[0]);
	}
	free(vars_values);
}

void	export(t_cmd *cmd, t_env *env)
{
	int		i;
	char	**vars_values;

	i = 1;
	if (cmd->argv_top == 0)
	{
		ft_env(cmd, env);
		return ;
	}
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
			export_loop1(vars_values);
		else
			export_loop2(vars_values);
		i++;
	}
	env->exit_code = EXIT_SUCCESS;
}
