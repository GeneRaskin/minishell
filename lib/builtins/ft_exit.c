/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:12:53 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:18:08 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins_private.h"

static void	terminate(t_env *env, int exit_code)
{
	free_env_vars(env->env_vars);
	free_env_vars(env->global_env_vars);
	free_parse_tree(env->parse_tree);
	clear_history();
	exit(exit_code);
}

static void	check_argc(t_cmd *cmd, int *exit_code)
{
	if (cmd->argv_top > 1)
	{
		ft_putendl_fd("exit: too many args", STDERR_FILENO);
		*exit_code = EXIT_FAILURE;
	}
}

void	ft_exit(t_cmd *cmd, t_env *env)
{
	int	i;
	int	exit_code;

	i = 0;
	exit_code = EXIT_SUCCESS;
	check_argc(cmd, &exit_code);
	if (cmd->argv_top == 1)
	{
		while (cmd->argv[1][i])
		{
			if (!ft_isdigit(cmd->argv[1][i]))
			{
				ft_putendl_fd("exit: use numeric code", STDERR_FILENO);
				exit_code = EXIT_FAILURE;
				break ;
			}
			i++;
		}
	}
	if (cmd->argv_top == 1 && !(cmd->argv[1][i]))
		exit_code = ft_atoi(cmd->argv[1]);
	terminate(env, exit_code);
}
