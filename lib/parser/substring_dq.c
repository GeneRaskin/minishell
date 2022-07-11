/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substring_dq.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 02:52:24 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/15 22:23:29 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"
#include "../../include/libft.h"
#include "../../include/env_vars.h"

void	item(t_env *env, t_curr_items_ptrs *ptrs);

static void	match_var(t_env *env, char **substr_dq)
{
	int		has_key;
	char	*temp;
	char	*key;
	char	*joined;

	has_key = 1;
	temp = *substr_dq;
	key = ft_substr(env->yytext, 0, env->yyleng);
	*substr_dq = get(key, env->env_vars);
	if (!*substr_dq)
		*substr_dq = get(key, env->global_env_vars);
	if (!*substr_dq)
	{
		*substr_dq = ft_strdup("");
		has_key = 0;
	}
	joined = ft_strjoin(temp, *substr_dq);
	free(temp);
	free(key);
	if (!has_key)
		free(*substr_dq);
	*substr_dq = joined;
	advance(env, 0);
}

static int	dollar_sign_case(t_env *env, char **substr_dq)
{
	char	*joined;
	char	*temp;

	if (match(DOLLAR_SIGN, env))
	{
		advance(env, 0);
		if (match(VAR, env))
			match_var(env, substr_dq);
		else if (match(QUESTION_MARK, env))
		{
			advance(env, 0);
			temp = ft_itoa(env->exit_code);
			joined = ft_strjoin(*substr_dq, temp);
			free(*substr_dq);
			free(temp);
			*substr_dq = joined;
		}
		else
		{
			env->error_custom_msg = SYNTAX_ERR;
			free(*substr_dq);
			return (0);
		}
	}
	return (1);
}

static void	substr_case(t_env *env, char **substr_dq)
{
	char	*temp;
	char	*joined;

	if (match(SUBSTRING, env))
	{
		temp = *substr_dq;
		*substr_dq = ft_substr(env->yytext, 0, env->yyleng);
		joined = ft_strjoin(temp, *substr_dq);
		free(temp);
		free(*substr_dq);
		*substr_dq = joined;
		advance(env, 0);
	}
}

char	*substring_dq(t_env *env)
{
	char	*substr_dq;

	if (match(DOUBLE_QUOTE, env))
	{
		substr_dq = ft_substr(env->yytext, 0, 0);
		if (!substr_dq)
			set_err_func_name(env, "malloc");
		advance(env, 0);
		return (substr_dq);
	}
	substr_dq = ft_strdup("");
	while (!match(DOUBLE_QUOTE, env))
	{
		if (!dollar_sign_case(env, &substr_dq))
			return (NULL);
		substr_case(env, &substr_dq);
		if (match(NEWLINE, env) || match(EOI, env))
		{
			free(substr_dq);
			env->error_custom_msg = SYNTAX_ERR;
			return (NULL);
		}
	}
	advance(env, 0);
	return (substr_dq);
}
