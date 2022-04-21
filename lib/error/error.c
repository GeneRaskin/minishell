#include "../../include/error.h"
#include "../../include/env_state.h"
#include "../../include/tokens.h"
#include "../../include/lex.h"
#include "../../include/libft.h"
#include <stdio.h>
#include <stdarg.h>

void	set_err_custom_msg(t_env *env, char *str)
{
	env->error_custom_msg = str;
}

void	set_err_func_name(t_env *env, char *str)
{
	env->error_func_name = str;
}

void	error(t_env *env)
{
	if (env->error_func_name)
	{
		perror(env->error_func_name);
		env->error_func_name = NULL;
	}
	else if (env->error_custom_msg)
	{
		ft_putendl_fd(env->error_custom_msg, STDERR_FILENO);
		env->error_custom_msg = NULL;
	}
}

int	legal_lookahead(t_env *env, ...)
{
	va_list	legal_tokens;
	int		curr_token;

	va_start(legal_tokens, env);
	curr_token = va_arg(legal_tokens, int);
	while (curr_token != NULL_TOKEN)
	{
		if (match(curr_token, env))
		{
			va_end(legal_tokens);
			return (1);
		}
		curr_token = va_arg(legal_tokens, int);
	}
	va_end(legal_tokens);
	return (0);
}
