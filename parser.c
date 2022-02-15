#include "minishell.h"

void	item(t_env *env);

t_cmd	*init_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *) malloc(sizeof(t_cmd));
	new_cmd->binname = NULL;
	new_cmd->in_filename = NULL;
	new_cmd->out_filename = NULL;
	new_cmd->append_mode = 0;
	new_cmd->delimeter = NULL;
	return (new_cmd);
}

t_cmd	*get_last_cmd(t_env *env)
{
	return (env->cmd_table->cmd_arr[env->cmd_table->top]);
}

void	substring_dq(t_env *env)
{
	advance(env, 0);
	while (!match(DOUBLE_QUOTE, env))
	{
		if (match(DOLLAR_SIGN, env))
		{
			advance(env, 0);
			if (match(LP, env))
			{
				env->state &= ~DOUBLE_Q;
				item(env);
				env->state |= DOUBLE_Q;
			}
			else if (match(VAR, env))
				advance(env, 0);
		}
		while (match(SUBSTRING, env) || match(SPACE, env))
			advance(env, 0);
	}
}

void	substring(t_env *env)
{
	if (match(SUBSTRING, env))
		advance(env, 0);
	else if (match(SINGLE_QUOTE, env))
	{
		advance(env, 0);
		if (match(SINGLE_QUOTE, env))
			advance(env, 0);
		else
			return ;
	}
	else if (match(DOLLAR_SIGN, env))
	{
		advance(env, 0);
		if (match(LP, env))
			item(env);
		else if (match(VAR, env))
			advance(env, 0);
	}
	else if (match(DOUBLE_QUOTE, env))
	{
		substring_dq(env);
		if (match(DOUBLE_QUOTE, env))
		{
			env->state &= ~DOUBLE_Q;
			advance(env, 0);
		}
	}
}

void	word(t_env *env)
{
	if (match(HEREDOC, env) || (match(APPEND_FILE, env))
		|| match(IN_FILE, env) || match(OUT_FILE, env))
		advance(env, 1);
	substring(env);
	while (match(SUBSTRING, env) || match(DOUBLE_QUOTE, env)
		|| match(SINGLE_QUOTE, env))
		substring(env);
}

void	item(t_env *env)
{
	env->cmd_table->cmd_arr[env->cmd_table->top++] = init_cmd();
	if (match(LP, env))
	{
		advance(env, 1);
		statements(env);
		if (match(RP, env))
			advance(env, 1);
		return ;
	}
	word(env);
	while (match(SPACE, env))
	{
		advance(env, 1);
		word(env);
	}
}

void	command(t_env *env)
{
	item(env);
	while (match(AND, env) || match(OR, env))
	{
		advance(env, 1);
		item(env);
	}
}

void	expression(t_env *env)
{
	command(env);
	while (match(PIPE, env))
	{
		advance(env, 1);
		command(env);
	}
}

void	statements(t_env *env)
{
	while (!match(EOI, env))
	{
		expression(env);
		if (match(SEMI, env))
			advance(env, 1);
		else
			return ;
	}
}