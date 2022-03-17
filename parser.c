#include "minishell.h"

void	item(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs);

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

void	word(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs)
{
	char		*word;
	char		*temp;
	t_cmd		*curr_cmd;

	curr_cmd = ptrs->curr_pipelst->u_item.cmd;
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

void	item(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs)
{
	if (match(LP, env))
	{
		advance(env, 1);
		ptrs->curr_pipelst->u_item.script = statements(env);
		ptrs->curr_pipelst->type = NEXT_SCRIPT;
		if (match(RP, env))
			advance(env, 1);
		return ;
	}
	ptrs->curr_pipelst->u_item.cmd = init_cmd();
	ptrs->curr_pipelst->type = NEXT_PIPELST;
	word(env, scripts, ptrs);
	while (match(SPACE, env))
	{
		advance(env, 1);
		word(env, scripts, ptrs);
	}
}

void	command(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs)
{
	t_cmd_table		*curr_cmd_table;

	curr_cmd_table = ptrs->curr_cmd_table;
	curr_cmd_table->pipelist = (t_pipelist *) malloc(
			sizeof(t_pipelist));
	curr_cmd_table->pipelist->next = NULL;
	ptrs->curr_pipelst = curr_cmd_table->pipelist;
	item(env, scripts, ptrs);
	while (match(PIPE, env))
	{
		ptrs->curr_pipelst->next = (t_pipelist *) malloc(
				sizeof(t_pipelist));
		ptrs->curr_pipelst = ptrs->curr_pipelst->next;
		ptrs->curr_pipelst->next = NULL;
		advance(env, 1);
		item(env, scripts, ptrs);
	}
}

void	expression(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs)
{
	t_scripts		*curr_script;

	curr_script = ptrs->curr_script;
	curr_script->cmd_table = (t_cmd_table *) malloc(sizeof(t_cmd_table));
	ptrs->curr_cmd_table = curr_script->cmd_table;
	command(env, scripts, ptrs);
	ptrs->curr_cmd_table->logical_op = 0;
	while (match(AND, env) || match(OR, env))
	{
		ptrs->curr_cmd_table->next = (t_cmd_table *)
			malloc(sizeof(t_cmd_table));
		ptrs->curr_cmd_table = ptrs->curr_cmd_table->next;
		if (match(AND, env))
			ptrs->curr_cmd_table->logical_op = AND;
		else if (match(OR, env))
			ptrs->curr_cmd_table->logical_op = OR;
		advance(env, 1);
		command(env, scripts, ptrs);
	}
}

t_scripts	*statements(t_env *env)
{
	t_scripts			*scripts;
	t_curr_items_ptrs	ptrs;

	if (!match(NEWLINE, env) && !match(EOI, env))
	{
		scripts = (t_scripts *) malloc(sizeof(t_scripts));
		scripts->next = NULL;
		ptrs.curr_script = scripts;
	}
	while (!match(NEWLINE, env) && !match(EOI, env))
	{
		expression(env, scripts, &ptrs);
		if (match(SEMI, env))
		{
			ptrs.curr_script->next = (t_scripts *) malloc(
					sizeof(t_scripts));
			ptrs.curr_script = ptrs.curr_script->next;
			ptrs.curr_script->next = NULL;
			advance(env, 1);
		}
		else
			return (scripts);
	}
	return (NULL);
}
