#include "minishell.h"

void	item(t_env *env, t_scripts *scripts);

t_cmd	*init_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *) malloc(sizeof(t_cmd));
	new_cmd->argv_top = -1;
	new_cmd->out_filename = NULL;
	new_cmd->in_filename = NULL;
	new_cmd->append_mode = 0;
	new_cmd->heredocs_top = -1;
	return (new_cmd);
}

t_cmd	*get_last_cmd(t_scripts *scripts)
{
	t_pipelist	*curr_pipelst;

	curr_pipelst = scripts->curr_pipelst;
	return (curr_pipelst->u_item.cmd);
}

char	*substring_dq(t_env *env)
{
	char	*substr_dq;
	char	*temp;

	if (match(DOUBLE_QUOTE, env))
	{
		substr_dq = ft_substr(env->yytext, 0, 0);
		advance(env, 0);
		return (substr_dq);
	}
	if (match(SUBSTRING, env))
		substr_dq = ft_substr(env->yytext, 0, env->yyleng);
	advance(env, 0);
	while (!match(DOUBLE_QUOTE, env))
	{
		if (match(DOLLAR_SIGN, env))
		{
			advance(env, 0);
			if (match(LP, env))
			{
				env->state &= ~DOUBLE_Q;
				advance(env, 1);
				statements(env);
				env->state |= DOUBLE_Q;
			}
			else if (match(VAR, env))
				advance(env, 0);
		}
		while (match(SUBSTRING, env))
		{
			temp = substr_dq;
			substr_dq = ft_substr(env->yytext, 0, env->yyleng);
			substr_dq = ft_strjoin(temp, substr_dq);
			free(temp);
			advance(env, 0);
		}
	}
	advance(env, 0);
	return (substr_dq);
}

char	*substring(t_env *env)
{
	char	*substr;

	if (match(SUBSTRING, env))
	{
		substr = ft_substr(env->yytext, 0, env->yyleng);
		advance(env, 0);
		return (substr);
	}
	else if (match(SINGLE_QUOTE, env))
	{
		advance(env, 0);
		if (match(SUBSTRING, env))
		{
			substr = ft_substr(env->yytext, 0, env->yyleng);
			advance(env, 0);
		}
		else if (match(SINGLE_QUOTE, env))
			substr = ft_substr(env->yytext, 0, 0);
		advance(env, 0);
		return (substr);
	}
	else if (match(DOLLAR_SIGN, env))
	{
		advance(env, 0);
		if (match(LP, env))
		{
			advance(env, 1);
			statements(env);
		}
		else if (match(VAR, env))
			advance(env, 0);
	}
	else if (match(DOUBLE_QUOTE, env))
	{
		advance(env, 0);
		substr = substring_dq(env);
		return (substr);
	}
	return (NULL);
}

void	word(t_env *env, t_scripts *scripts)
{
	char		*word;
	char		*temp;
	t_cmd		*curr_cmd;

	curr_cmd = get_last_cmd(scripts);
	if (match(HEREDOC, env) || (match(APPEND_FILE, env))
		|| match(IN_FILE, env) || match(OUT_FILE, env))
	{
		if (match(APPEND_FILE, env))
			curr_cmd->append_mode = 1;
		else if (match(IN_FILE, env))
			env->state |= IN_FILE_TK;
		else if (match(OUT_FILE, env))
			env->state |= OUT_FILE_TK;
		else if (match(HEREDOC, env))
			env->state |= HEREDOC_TK;
		advance(env, 1);
	}
	word = substring(env);
	while (match(SUBSTRING, env) || match(DOUBLE_QUOTE, env)
		|| match(SINGLE_QUOTE, env))
	{
		temp = word;
		word = substring(env);
		word = ft_strjoin(temp, word);
		free(temp);
	}
	if (env->state & IN_FILE_TK)
		curr_cmd->in_filename = word;
	else if ((env->state & OUT_FILE_TK || curr_cmd->append_mode)
		&& !curr_cmd->out_filename)
		curr_cmd->out_filename = word;
	else if (env->state & HEREDOC_TK)
		curr_cmd->delimeters[++curr_cmd->heredocs_top] = word;
	else
		curr_cmd->argv[++curr_cmd->argv_top] = word;
	env->state &= ~IN_FILE_TK;
	env->state &= ~OUT_FILE_TK;
	env->state &= ~HEREDOC_TK;
}

void	item(t_env *env, t_scripts *scripts)
{
	if (match(LP, env))
	{
		advance(env, 1);
		scripts->curr_pipelst->u_item.script = statements(env);
		scripts->curr_pipelst->type = NEXT_SCRIPT;
		if (match(RP, env))
			advance(env, 1);
		return ;
	}
	scripts->curr_pipelst->u_item.cmd = init_cmd();
	scripts->curr_pipelst->type = NEXT_PIPELST;
	word(env, scripts);
	while (match(SPACE, env))
	{
		advance(env, 1);
		word(env, scripts);
	}
}

void	command(t_env *env, t_scripts *scripts)
{
	t_scripts		*curr_script;
	t_cmd_table		*curr_cmd_table;

	curr_script = scripts->curr_script;
	curr_cmd_table = curr_script->cmd_table;
	curr_cmd_table->cmd_arr[++curr_cmd_table->top] = (t_pipelist *) malloc(
			sizeof(t_pipelist));
	curr_cmd_table->cmd_arr[curr_cmd_table->top]->next = NULL;
	scripts->curr_pipelst = curr_cmd_table->cmd_arr[curr_cmd_table->top];
	item(env, scripts);
	while (match(PIPE, env))
	{
		scripts->curr_pipelst->next = (t_pipelist *) malloc(
				sizeof(t_pipelist));
		scripts->curr_pipelst = scripts->curr_pipelst->next;
		scripts->curr_pipelst->next = NULL;
		advance(env, 1);
		item(env, scripts);
	}
}

void	expression(t_env *env, t_scripts *scripts)
{
	t_cmd_table		*curr_cmd_table;
	t_scripts		*curr_script;

	curr_script = scripts->curr_script;
	curr_script->cmd_table = (t_cmd_table *) malloc(sizeof(t_cmd_table));
	curr_cmd_table = curr_script->cmd_table;
	curr_cmd_table->top = -1;
	command(env, scripts);
	curr_cmd_table->logical_op[0] = 0;
	while (match(AND, env) || match(OR, env))
	{
		if (match(AND, env))
			curr_cmd_table->logical_op[curr_cmd_table->top + 1] = AND;
		else if (match(OR, env))
			curr_cmd_table->logical_op[curr_cmd_table->top + 1] = OR;
		advance(env, 1);
		command(env, scripts);
	}
}

t_scripts	*statements(t_env *env)
{
	t_scripts	*scripts;

	scripts = (t_scripts *) malloc(sizeof(t_scripts));
	scripts->next = NULL;
	scripts->curr_script = scripts;
	while (!match(EOI, env))
	{
		expression(env, scripts);
		if (match(SEMI, env))
		{
			scripts->curr_script->next = (t_scripts *) malloc(
					sizeof(t_scripts));
			scripts->curr_script = scripts->curr_script->next;
			scripts->curr_script->next = NULL;
			advance(env, 1);
		}
		else
			return (scripts);
	}
}
