#include "include/env_state.h"
#include "include/libft.h"
#include "include/parser.h"
#include "include/lex.h"
#include "include/tokens.h"

static void	reset_tokens(t_env *env)
{
	env->lookahead = -1;
	env->yyleng = 0;
}

static void	expansion_loop(t_env *env, char **expanded_str)
{
	int		curr_len;
	char	*temp;
	char	*temp2;

	curr_len = 0;
	while ((env->yytext[curr_len] != '$' || env->state & SINGLE_Q)
		&& env->yytext[curr_len])
	{
		if (env->yytext[curr_len] == '\'' && !(env->state & SINGLE_Q))
			env->state |= SINGLE_Q;
		else if (env->yytext[curr_len] == '\'' && (env->state & SINGLE_Q))
			env->state &= ~SINGLE_Q;
		curr_len++;
	}
	temp = ft_substr(env->yytext, 0, curr_len);
	temp2 = *expanded_str;
	*expanded_str = ft_strjoin(*expanded_str, temp);
	free(temp);
	free(temp2);
	env->yytext += curr_len;
}

void	expand_dollar(t_env *env)
{
	char	*expanded_str;
	char	*temp;
	char	*temp2;

	expanded_str = ft_strdup("");
	while (*(env->yytext))
	{
		expansion_loop(env, &expanded_str);
		if (*env->yytext == '$' && !(env->state & SINGLE_Q))
		{
			advance(env, 0);
			temp = dollar_expansion(env);
			temp2 = expanded_str;
			expanded_str = ft_strjoin(expanded_str, temp);
			free(temp);
			free(temp2);
		}
		if (match(EOI, env))
			break ;
		reset_tokens(env);
	}
	reset_tokens(env);
	env->state = 0;
	env->yytext = expanded_str;
}
