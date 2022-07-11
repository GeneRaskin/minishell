/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:41:43 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:42:25 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/env_state.h"
#include "include/env_vars.h"
#include "include/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void	init_env(t_env *env);

int	init_iter(t_env *env)
{
	init_env(env);
	if (env->yytext && *(env->yytext))
		add_history(env->yytext);
	if (!env->yytext)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		free_env_vars(env->env_vars);
		free_env_vars(env->global_env_vars);
		clear_history();
		exit(EXIT_SUCCESS);
	}
	else if (!*(env->yytext))
		return (0);
	return (1);
}
