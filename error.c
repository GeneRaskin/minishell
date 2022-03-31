#include "minishell.h"

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
		perror(env->error_func_name);
	else if (env->error_custom_msg)
		ft_putendl_fd(env->error_custom_msg, STDERR_FILENO);
}

int	legal_lookahead(t_env *env, int token, ...)
{
	va_list	legal_tokens;
	int		curr_token;
	int		checked_token;

	va_start(legal_tokens, token);
	curr_token = va_arg(legal_tokens, int);
	checked_token = env->lookahead;
	while (curr_token != -1)
	{
		if (curr_token == checked_token)
		{
			va_end(legal_tokens);
			return (1);
		}
		curr_token = va_arg(legal_tokens, int);
	}
	va_end(legal_tokens);
	return (0);
}
