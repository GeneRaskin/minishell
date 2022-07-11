/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_unset.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:12:46 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:12:47 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins_private.h"

void	ft_env(t_cmd *cmd, t_env *env)
{
	t_env_vars	*vars;

	if (cmd->argv_top != 0)
	{
		env->error_custom_msg = ENV_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	vars = env->global_env_vars;
	while (vars != NULL)
	{
		printf("%s=%s\n", vars->name, vars->value);
		vars = vars->next;
	}
	env->exit_code = EXIT_SUCCESS;
}

void	ft_unset(t_cmd *cmd, t_env *env)
{
	int	i;

	i = 1;
	if (cmd->argv_top < 1)
	{
		env->error_custom_msg = UNSET_ARGC_ERROR;
		env->exit_code = EXIT_FAILURE;
		return ;
	}
	while (i <= cmd->argv_top)
	{
		if (get(cmd->argv[i], env->global_env_vars))
			unset(cmd->argv[i], &(env->global_env_vars));
		else
			unset(cmd->argv[i], &(env->env_vars));
		i++;
	}
	env->exit_code = EXIT_SUCCESS;
}
