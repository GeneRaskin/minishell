#include "../../include/env_state.h"
#include "../../include/tokens.h"
#include "../../include/libft.h"
#include "../../include/error.h"

int	dollar(char **current, t_env *env);

static int	loop_no_quotes(char **current, t_env *env)
{
	while (**current && !ft_isspace(**current)
		&& **current != '>' && **current != '<'
		&& **current != '|' && **current != '\''
		&& **current != '"' && **current != '&'
		&& **current != '$' && **current != ';'
		&& **current != ')' && **current != '('
		&& **current != '\n')
		++(*current);
	if (**current == '&' && *(*current + 1) != '&')
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (NULL_TOKEN);
	}
	env->yyleng = *current - env->yytext;
	return (SUBSTRING);
}

static int	simple_tokens(char **current, t_env *env)
{
	if (**current == ';')
		return (SEMI);
	else if (**current == '(')
		return (LP);
	else if (**current == '\n')
		return (NEWLINE);
	else if (**current == ')')
		return (RP);
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
	else
	{
		env->state |= DOLLAR;
		return (DOLLAR_SIGN);
	}
}

static int	here_in_token(char **current, t_env *env)
{
	if (*(*current + 1) == '<')
	{
		env->yyleng++;
		return (HEREDOC);
	}
	return (IN_FILE);
}

static int	compound_tokens(char **current, t_env *env)
{
	if (**current == '|')
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
	else
		return (here_in_token(current, env));
}

int	no_quotes(char **current, t_env *env)
{
	if (**current == ';' || **current == '(' || **current == '\n'
		|| **current == ')' || ft_isspace(**current) || **current == '"'
		|| **current == '\'' || **current == '$')
		return (simple_tokens(current, env));
	else if (**current == '|' || **current == '>' || **current == '<')
		return (compound_tokens(current, env));
	else if (**current == '&' && *(*current + 1) == '&')
	{
		env->yyleng++;
		return (AND);
	}
	else if (**current == '=' && env->state & PARSE_VAR)
		return (EQUAL_SIGN);
	else if (**current == '+' && (env->state & PARSE_VAR
			&& env->state & APPEND_TO_VAR))
		return (PLUS_SIGN);
	else if (env->state & DOLLAR)
		return (dollar(current, env));
	else
		return (loop_no_quotes(current, env));
}
