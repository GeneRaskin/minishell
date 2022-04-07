#include "../../include/libft.h"
#include "../../include/tokens.h"
#include "../../include/env_state.h"

int	no_quotes(char **current, t_env *env);

static int	single_q(char **current, t_env *env)
{
	while (**current && **current != '\''
		   && **current != '\n')
		++(*current);
	env->yyleng = *current - env->yytext;
	if (**current == '\n')
		return (NEWLINE);
	if (env->yyleng == 0)
	{
		if (**current == '\'')
		{
			env->state &= ~SINGLE_Q;
			env->yyleng = 1;
			return (SINGLE_QUOTE);
		}
	}
	return (SUBSTRING);
}

int	dollar(char **current, t_env *env)
{
	env->state &= ~DOLLAR;
	if (**current == '(')
		return (LP);
	else
	{
		if (!ft_isalpha(**current))
			return (NULL_TOKEN);
		while (**current && ft_isalnum(**current))
			++(*current);
		env->yyleng = *current - env->yytext;
		if (env->yyleng == 0)
			return (NULL_TOKEN);
		return (VAR);
	}
}

static int	double_quotes(char **current, t_env *env)
{
	if (**current == '$')
	{
		env->state |= DOLLAR;
		return (DOLLAR_SIGN);
	}
	if (env->state & DOLLAR)
		return (dollar(current, env));
	while (**current && **current != '"' && **current != '$'
		   && **current != '\n')
		++(*current);
	env->yyleng = *current - env->yytext;
	if (**current == '\n')
		return (NEWLINE);
	if (env->yyleng == 0)
	{
		if (**current == '"')
		{
			env->state &= ~DOUBLE_Q;
			env->yyleng = 1;
			return (DOUBLE_QUOTE);
		}
	}
	return (SUBSTRING);
}

int	which_token(char **current, t_env *env)
{
	if (!(env->state & SINGLE_Q) && !(env->state & DOUBLE_Q))
	{
		return (no_quotes(current, env));
	}
	else if (env->state & SINGLE_Q)
		return (single_q(current, env));
	else if (env->state & DOUBLE_QUOTE)
		return (double_quotes(current, env));
	return (NULL_TOKEN);
}
