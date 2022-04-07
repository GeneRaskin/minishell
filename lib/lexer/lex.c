#include "../../include/lex.h"

int	which_token(char **current, t_env *env)
{
	if (!(env->state & SINGLE_Q) && !(env->state & DOUBLE_Q))
	{
		if (**current == ';')
			return (SEMI);
		else if (**current == '(')
			return (LP);
		else if (**current == '\n')
			return (NEWLINE);
		else if (**current == ')')
			return (RP);
		else if (**current == '|')
		{
			if (*(*current + 1) == '|')
			{
				env->yyleng++;
				return (OR);
			}
			return (PIPE);
		}
		else if (**current == '>')
		{
			if (*(*current + 1) == '>')
			{
				env->yyleng++;
				return (APPEND_FILE);
			}
			return (OUT_FILE);
		}
		else if (**current == '<')
		{
			if (*(*current + 1) == '<')
			{
				env->yyleng++;
				return (HEREDOC);
			}
			return (IN_FILE);
		}
		else if (**current == '&' && *(*current + 1) == '&')
		{
			env->yyleng++;
			return (AND);
		}
		else if (**current == '=')
			return (EQUAL_SIGN);
		else if (ft_isspace(**current))
			return (SPACE);
		else if (**current == '"')
		{
			env->state |= DOUBLE_Q;
			return (DOUBLE_QUOTE);
		}
		else if (**current == '\'')
		{
			env->state |= SINGLE_Q;
			return (SINGLE_QUOTE);
		}
		else if (**current == '$')
			return (DOLLAR_SIGN);
		else
		{
			while (**current && !ft_isspace(**current)
				&& **current != '>' && **current != '<'
				&& **current != '|' && **current != '\''
				&& **current != '"' && **current != '&'
				&& **current != '$' && **current != ';'
				&& **current != ')' && **current != '('
				&& **current != '\n')
				++(*current);
			env->yyleng = *current - env->yytext;
			return (SUBSTRING);
		}
	}
	else if (env->state & SINGLE_Q)
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
	else if (env->state & DOUBLE_QUOTE)
	{
		if (**current == '$')
		{
			env->state |= DOLLAR;
			return (DOLLAR_SIGN);
		}
		if (env->state & DOLLAR)
		{
			env->state &= ~DOLLAR;
			if (**current == '(')
				return (LP);
			else
			{
				while (**current && ft_isalnum(**current))
					++(*current);
				env->yyleng = *current - env->yytext;
				return (VAR);
			}
		}
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
	return (NULL_TOKEN);
}

int	lex(t_env *env, int skip_spaces)
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
