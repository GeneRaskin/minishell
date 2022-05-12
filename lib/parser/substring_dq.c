#include "parser_private.h"
#include "../../include/libft.h"
#include "../../include/env_vars.h"

void	item(t_env *env, t_curr_items_ptrs *ptrs);

char	*substring_dq(t_env *env)
{
	char	*substr_dq;
	char	*temp;
	char	*key;
	char	*joined;
	int		has_key;

	has_key = 1;
	if (match(DOUBLE_QUOTE, env))
	{
		substr_dq = ft_substr(env->yytext, 0, 0);
		if (!substr_dq)
			set_err_func_name(env, "malloc");
		advance(env, 0);
		return (substr_dq);
	}
	substr_dq = ft_strdup("");
	if (!substr_dq)
	{
		set_err_func_name(env, "malloc");
		return (NULL);
	}
	while (!match(DOUBLE_QUOTE, env))
	{
		if (match(DOLLAR_SIGN, env))
		{
			advance(env, 0);
			if (match(VAR, env))
			{
				temp = substr_dq;
				key = ft_substr(env->yytext, 0, env->yyleng);
				if (!key)
				{
					env->error_func_name = "malloc";
					free(substr_dq);
					return (NULL);
				}
				substr_dq = get(key, env->env_vars);
				if (!substr_dq)
					substr_dq = get(key, env->global_env_vars);
				if (!substr_dq)
				{
					substr_dq = ft_strdup("");
					has_key = 0;
					if (!substr_dq)
					{
						free(temp);
						free(key);
						env->error_func_name = "malloc";
						return (NULL);
					}
				}
				joined = ft_strjoin(temp, substr_dq);
				if (!joined)
				{
					set_err_func_name(env, "malloc");
					free(key);
					free(temp);
					if (!has_key)
						free(substr_dq);
					return (NULL);
				}
				free(temp);
				free(key);
				if (!has_key)
					free(substr_dq);
				substr_dq = joined;
				advance(env, 0);
			}
			else if (match(QUESTION_MARK, env))
			{
				advance(env, 0);
				temp = ft_itoa(env->exit_code);
				joined = ft_strjoin(substr_dq, temp);
				free(substr_dq);
				free(temp);
				substr_dq = joined;
			}
			else
			{
				env->error_custom_msg = SYNTAX_ERR;
				free(substr_dq);
				return (NULL);
			}
		}
		if (match(SUBSTRING, env))
		{
			temp = substr_dq;
			substr_dq = ft_substr(env->yytext, 0, env->yyleng);
			if (!substr_dq)
			{
				set_err_func_name(env, "malloc");
				free(temp);
				return (NULL);
			}
			joined = ft_strjoin(temp, substr_dq);
			if (!joined)
			{
				set_err_func_name(env, "malloc");
				free(temp);
				free(substr_dq);
				return (NULL);
			}
			free(temp);
			free(substr_dq);
			substr_dq = joined;
			advance(env, 0);
		}
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
