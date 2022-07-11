/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:37:09 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:37:11 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lex.h"
#include "../../include/tokens.h"
#include "../../include/env_state.h"

int	which_token(char **current, t_env *env);

static int	lex(t_env *env, int skip_spaces)
{
	char		*current;
	int			curr_token;

	current = env->yytext + env->yyleng;
	while (*current)
	{
		env->yytext = current;
		env->yyleng = 1;
		curr_token = which_token(&current, env);
		if (curr_token != SPACE || !skip_spaces)
			return (curr_token);
		current++;
	}
	return (EOI);
}

int	match(int token, t_env *env)
{
	if (env->lookahead == -1)
		env->lookahead = lex(env, 1);
	return (token == env->lookahead);
}

void	advance(t_env *env, int skip_spaces)
{
	env->lookahead = lex(env, skip_spaces);
}
