/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substring.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 02:52:18 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/15 22:33:38 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"
#include "../../include/libft.h"
#include "../../include/env_vars.h"

char	*substring_dq(t_env *env);

static char	*single_q(t_env *env)
{
	char	*substr;

	advance(env, 0);
	if (match(SUBSTRING, env))
	{
		substr = ft_substr(env->yytext, 0, env->yyleng);
		advance(env, 0);
		if (!match(SINGLE_QUOTE, env))
		{
			env->error_custom_msg = SYNTAX_ERR;
			free(substr);
			return (NULL);
		}
	}
	else if (match(SINGLE_QUOTE, env))
		substr = ft_substr(env->yytext, 0, 0);
	else
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (NULL);
	}
	advance(env, 0);
	return (substr);
}

static char	*match_var(t_env *env)
{
	char	*key;
	char	*substr;

	key = ft_substr(env->yytext, 0, env->yyleng);
	if (!get(key, env->global_env_vars))
	{
		substr = get(key, env->env_vars);
		if (substr)
			substr = ft_strdup(substr);
	}
	else
	{
		substr = get(key, env->global_env_vars);
		if (substr)
			substr = ft_strdup(substr);
	}
	if (!substr)
		substr = ft_strdup("");
	advance(env, 0);
	free(key);
	return (substr);
}

char	*dollar_expansion(t_env *env)
{
	advance(env, 0);
	if (match(VAR, env))
		return (match_var(env));
	else if (match(QUESTION_MARK, env))
	{
		advance(env, 0);
		return (ft_itoa(env->exit_code));
	}
	else
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (NULL);
	}
}

static char	*call_substring_dq(t_env *env)
{
	char	*substr;

	advance(env, 0);
	substr = substring_dq(env);
	return (substr);
}

char	*substring(t_env *env)
{
	char		*substr;

	if (!legal_lookahead(env, SUBSTRING, SINGLE_QUOTE,
			DOUBLE_QUOTE, DOLLAR_SIGN, NULL_TOKEN))
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (NULL);
	}
	if (match(SUBSTRING, env))
	{
		substr = ft_substr(env->yytext, 0, env->yyleng);
		if (!substr)
			set_err_func_name(env, "malloc");
		advance(env, 0);
		return (substr);
	}
	else if (match(SINGLE_QUOTE, env))
		return (single_q(env));
	else if (match(DOLLAR_SIGN, env))
		return (dollar_expansion(env));
	else
		return (call_substring_dq(env));
}
