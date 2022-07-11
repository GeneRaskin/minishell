/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:12:20 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:18:08 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins_private.h"

static int	check_flag(char *s)
{
	if (*(s++) == '-')
	{
		while (*s == 'n')
			s++;
		if (!*s)
			return (1);
	}
	return (0);
}

void	echo(t_cmd *cmd, t_env *env)
{
	int	flag;
	int	i;

	flag = 0;
	i = 1;
	if (cmd->argv_top >= 1)
		flag = check_flag(cmd->argv[1]);
	if (flag)
		i++;
	while (i <= cmd->argv_top)
	{
		printf("%s", cmd->argv[i]);
		if (i != cmd->argv_top)
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	env->exit_code = EXIT_SUCCESS;
}
