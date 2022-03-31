#include "minishell.h"

void	item(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs);

t_cmd	*init_cmd(t_env *env)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *) malloc(sizeof(t_cmd));
	if (!new_cmd)
	{
		set_err_func_name(env, "malloc");
		return (NULL);
	}
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
		if (!substr_dq)
			set_err_func_name(env, "malloc");
		advance(env, 0);
		return (substr_dq);
	}
	if (match(SUBSTRING, env))
	{
		substr_dq = ft_substr(env->yytext, 0, env->yyleng);
		if (!substr_dq)
		{
			set_err_func_name(env, "malloc");
			return (NULL);
		}
	}
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
			if (!substr_dq)
			{
				set_err_func_name(env, "malloc");
				free(temp);
				return (NULL);
			}
			substr_dq = ft_strjoin(temp, substr_dq);
			if (!substr_dq)
			{
				set_err_func_name(env, "malloc");
				free(temp);
				return (NULL);
			}
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
		if (!substr)
			set_err_func_name(env, "malloc");
		advance(env, 0);
		return (substr);
	}
	else if (match(SINGLE_QUOTE, env))
	{
		advance(env, 0);
		if (match(SUBSTRING, env))
		{
			substr = ft_substr(env->yytext, 0, env->yyleng);
			if (!substr)
				set_err_func_name(env, "malloc");
			advance(env, 0);
		}
		else if (match(SINGLE_QUOTE, env))
		{
			substr = ft_substr(env->yytext, 0, 0);
			if (!substr)
				set_err_func_name(env, "malloc");
		}
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
	if (env->error_custom_msg || env->error_func_name)
		return ;
	while (match(SUBSTRING, env) || match(DOUBLE_QUOTE, env)
		|| match(SINGLE_QUOTE, env))
	{
		temp = word;
		word = substring(env);
		if (env->error_custom_msg || env->error_func_name)
			return ;
		word = ft_strjoin(temp, word);
		if (!word)
		{
			free(temp);
			set_err_func_name(env, "malloc");
			return ;
		}
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
		env->opened_parens++;
		advance(env, 1);
		ptrs->curr_pipelst->u_item.script = statements(env);
		if (env->error_custom_msg || env->error_func_name)
			return ;
		ptrs->curr_pipelst->type = NEXT_SCRIPT;
		if (match(RP, env))
		{
			env->opened_parens--;
			advance(env, 1);
		}
	}
	ptrs->curr_pipelst->u_item.cmd = init_cmd(env);
	if (env->error_func_name)
		return ;
	ptrs->curr_pipelst->type = NEXT_PIPELST;
	word(env, scripts, ptrs);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	while (match(SPACE, env))
	{
		advance(env, 1);
		word(env, scripts, ptrs);
		if (env->error_custom_msg || env->error_func_name)
			return ;
	}
}

void	command(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs)
{
	t_cmd_table		*curr_cmd_table;

	curr_cmd_table = ptrs->curr_cmd_table;
	curr_cmd_table->pipelist = (t_pipelist *) malloc(
			sizeof(t_pipelist));
	if (!curr_cmd_table->pipelist)
	{
		set_err_func_name(env, "malloc");
		return ;
	}
	curr_cmd_table->pipelist->next = NULL;
	ptrs->curr_pipelst = curr_cmd_table->pipelist;
	item(env, scripts, ptrs);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	while (match(PIPE, env))
	{
		ptrs->curr_pipelst->next = (t_pipelist *) malloc(
				sizeof(t_pipelist));
		if (!ptrs->curr_pipelst->next)
		{
			set_err_func_name(env, "malloc");
			return ;
		}
		ptrs->curr_pipelst = ptrs->curr_pipelst->next;
		ptrs->curr_pipelst->next = NULL;
		advance(env, 1);
		item(env, scripts, ptrs);
		if (env->error_custom_msg || env->error_func_name)
			return ;
	}
}

void	expression(t_env *env, t_scripts *scripts, t_curr_items_ptrs *ptrs)
{
	t_scripts		*curr_script;

	curr_script = ptrs->curr_script;
	curr_script->cmd_table = (t_cmd_table *) malloc(sizeof(t_cmd_table));
	if (!curr_script->cmd_table)
	{
		set_err_func_name(env, "malloc");
		return ;
	}
	curr_script->cmd_table->next = NULL;
	ptrs->curr_cmd_table = curr_script->cmd_table;
	command(env, scripts, ptrs);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	ptrs->curr_cmd_table->logical_op = 0;
	while (match(AND, env) || match(OR, env))
	{
		ptrs->curr_cmd_table->next = (t_cmd_table *)
			malloc(sizeof(t_cmd_table));
		if (!ptrs->curr_cmd_table->next)
		{
			set_err_func_name(env, "malloc");
			return ;
		}
		ptrs->curr_cmd_table = ptrs->curr_cmd_table->next;
		ptrs->curr_cmd_table->next = NULL;
		if (match(AND, env))
			ptrs->curr_cmd_table->logical_op = AND;
		else if (match(OR, env))
			ptrs->curr_cmd_table->logical_op = OR;
		advance(env, 1);
		command(env, scripts, ptrs);
		if (env->error_custom_msg || env->error_func_name)
			return ;
	}
}

t_scripts	*statements(t_env *env)
{
	t_scripts			*scripts;
	t_curr_items_ptrs	ptrs;

	if (!match(NEWLINE, env) && !match(EOI, env))
	{
		scripts = (t_scripts *) malloc(sizeof(t_scripts));
		if (!scripts)
		{
			set_err_func_name(env, "malloc");
			return (scripts);
		}
		scripts->next = NULL;
		ptrs.curr_script = scripts;
	}
	else
		return (NULL);
	while (!match(NEWLINE, env) && !match(EOI, env))
	{
		expression(env, scripts, &ptrs);
		if (env->error_custom_msg || env->error_custom_msg)
			return (scripts);
		if (match(SEMI, env))
		{
			ptrs.curr_script->next = (t_scripts *) malloc(
					sizeof(t_scripts));
			if (!ptrs.curr_script->next)
			{
				set_err_func_name(env, "malloc");
				return (scripts);
			}
			ptrs.curr_script = ptrs.curr_script->next;
			ptrs.curr_script->next = NULL;
			advance(env, 1);
		}
	}
	return (scripts);
}
