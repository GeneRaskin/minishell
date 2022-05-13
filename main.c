/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 20:32:17 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 21:10:48 by lemmon           ###   ########.fr       */
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
void	loop(t_env *env);

void	init_env(t_env *env)
{
	env->lookahead = -1;
	env->yytext = "";
	env->yyleng = 0;
	env->state = 0;
	env->state |= 0x200;
	env->opened_parens = 0;
	env->error_func_name = NULL;
	env->error_custom_msg = NULL;
	env->parse_tree = NULL;
	env->yytext = readline(SHELL_NAME);
}

void	execute(t_env *env)
{
	if (env->error_func_name || env->error_custom_msg)
	{
		error(env);
		free_parse_tree(env->parse_tree);
		env->exit_code = EXIT_FAILURE;
	}
	else if (env->parse_tree)
	{
		env->state &= ~0x200;
		executor(env->parse_tree, env, STDIN_FILENO, STDOUT_FILENO);
		free_parse_tree(env->parse_tree);
	}
}

static	void	free_key_and_value(char **key_and_value, t_env *env)
{
	free(key_and_value[0]);
	free(key_and_value[1]);
	error(env);
}

static t_env_vars	*init_globals(t_env *env)
{
	char		**curr_var;
	char		**key_and_value;
	t_env_vars	*global_env_vars;

	curr_var = environ;
	global_env_vars = NULL;
	while (*curr_var != NULL)
	{
		key_and_value = ft_split(*curr_var, '=');
		if (key_and_value == NULL)
		{
			env->error_func_name = "malloc";
			error(env);
			curr_var++;
			continue ;
		}
		set(key_and_value[0], key_and_value[1], &(global_env_vars), env);
		if (env->error_func_name)
			free_key_and_value(key_and_value, env);
		free(key_and_value);
		curr_var++;
	}
	return (global_env_vars);
}

int	main(void)
{
	t_env	env;

	env.env_vars = NULL;
	env.error_func_name = NULL;
	env.error_custom_msg = NULL;
	env.exit_code = EXIT_SUCCESS;
	if (signal(SIGINT, catch_sigint) == SIG_ERR
		|| signal(SIGTSTP, SIG_IGN) == SIG_ERR
		|| signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		env.error_func_name = "signal";
		error(&env);
	}
	env.global_env_vars = init_globals(&env);
	env.state |= 0x200;
	g_env = &env;
	loop(&env);
}
