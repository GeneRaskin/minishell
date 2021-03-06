/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 02:12:06 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/14 02:12:07 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sigs.h"
#include "include/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>
#include "include/env_state.h"

extern t_env	*g_env;

void	catch_sigint(int signum)
{
	(void) signum;
	if (g_env->state & 0x100)
	{
		g_env->state |= 0x400;
	}
	else if (g_env->state & 0x200)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
	else
		ft_putstr_fd("\n", STDOUT_FILENO);
}
