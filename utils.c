/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 21:06:41 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 21:10:56 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/error.h"
#include "include/libft.h"
#include "include/free.h"
#include "include/executor.h"
#include "include/parser.h"
#include "include/env_state.h"
#include "include/env_vars.h"
#include "include/parse_tree.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <signal.h>
#include "sigs.h"
#define SHELL_NAME "gene_shell$ "

t_env	*g_env;
void	expand_dollar(t_env *env);
void	init_env(t_env *env);
void	execute(t_env *env);

static	void	free_env(t_env *env)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
	free_env_vars(env->env_vars);
	free_env_vars(env->global_env_vars);
	clear_history();
	exit(EXIT_SUCCESS);
}

static int	check_error(t_env *env, char *curr_line)
{
	if (env->error_custom_msg || env->error_func_name)
	{
		error(env);
		free(curr_line);
		return (1);
	}
	return (0);
}

static int	error_func(t_env *env)
{
	if (env->error_func_name)
	{
		error(env);
		return (1);
	}
	return (0);
}

void	loop(t_env *env)
{
	char			*curr_line;

	while (1)
	{
		init_env(env);
		if (env->yytext && *(env->yytext))
			add_history(env->yytext);
		if (!env->yytext)
			free_env(env);
		else if (!*(env->yytext))
			continue ;
		if (error_func(env))
			continue ;
		curr_line = env->yytext;
		while (ft_isspace(*(env->yytext)))
			env->yytext++;
		expand_dollar(env);
		if (check_error(env, curr_line))
			continue ;
		env->parse_tree = statements(env);
		free(curr_line);
		execute(env);
	}
}
