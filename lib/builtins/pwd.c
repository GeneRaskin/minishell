/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:12:58 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:18:08 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins_private.h"

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
