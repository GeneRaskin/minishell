#include "../../include/parser.h"
#include "../../include/tokens.h"
#include "../../include/parse_tree.h"
#include "../../include/env_state.h"
#include "../../include/error.h"
#include "../../include/lex.h"
#include "../../include/libft.h"
#include "../../include/env_vars.h"
#include "../../include/executor.h"
#include <stdio.h>

typedef struct s_curr_items_ptrs
{
	t_pipelist	*curr_pipelst;
	t_cmd_table	*curr_cmd_table;
	t_scripts	*curr_script;
}	t_curr_items_ptrs;

void		item(t_env *env, t_curr_items_ptrs *ptrs);

t_cmd	*init_cmd(t_env *env)
{
	t_cmd	*new_cmd;
	int		i;

	i = 0;
	new_cmd = (t_cmd *) malloc(sizeof(t_cmd));
	if (!new_cmd)
	{
		set_err_func_name(env, "malloc");
		return (NULL);
	}
	while (i < MAX_ARGV)
	{
		new_cmd->argv[i] = NULL;
		i++;
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
	char		*substr_dq;
	char		*temp;
	char		*key;

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
			if (match(LP, env))
			{
				env->opened_parens++;
				env->state &= ~DOUBLE_Q;
				advance(env, 1);
				statements(env);
				env->state |= DOUBLE_Q;
			}
			else if (match(VAR, env))
			{
				temp = substr_dq;
				key = ft_substr(env->yytext, 0, env->yyleng);
				if (!key)
				{
					env->error_func_name = "malloc";
					free(substr_dq);
					return (NULL);
				}
				substr_dq = get(key, env->env_vars, env);
				if (!substr_dq)
					substr_dq = ft_strdup("");
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
		if (match(NEWLINE, env) || match(EOI, env))
		{
			env->error_custom_msg = SYNTAX_ERR;
			return (NULL);
		}
	}
	advance(env, 0);
	return (substr_dq);
}

char	*substring(t_env *env)
{
	char		*substr;
	char		*key;
	t_scripts	*scripts;
	FILE		*tmp_file;

	if (!legal_lookahead(env, SUBSTRING, SINGLE_QUOTE,
			DOUBLE_QUOTE, DOLLAR_SIGN, NULL_TOKEN))
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (NULL);
	}
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
		else
		{
			env->error_custom_msg = SYNTAX_ERR;
			return (NULL);
		}
		advance(env, 0);
		return (substr);
	}
	else if (match(DOLLAR_SIGN, env))
	{
		advance(env, 0);
		if (match(LP, env))
		{
			env->state &= ~DOLLAR;
			env->opened_parens++;
			advance(env, 1);
			scripts = statements(env);
			if (env->error_custom_msg || env->error_func_name)
				return (NULL);
			tmp_file = tmpfile();
			executor(scripts, env, STDIN_FILENO, fileno(tmp_file));
		}
		else if (match(VAR, env))
		{
			key = ft_substr(env->yytext, 0, env->yyleng);
			if (!key)
			{
				env->error_func_name = "malloc";
				return (NULL);
			}
			substr = ft_strdup(get(key, env->env_vars, env));
			if (!substr)
				substr = ft_strdup("");
			advance(env, 0);
			return (substr);
		}
		else
		{
			env->error_custom_msg = SYNTAX_ERR;
			return (NULL);
		}
	}
	else
	{
		advance(env, 0);
		substr = substring_dq(env);
		return (substr);
	}
}

int	verify_var_name(t_env *env)
{
	char	*var;
	int		len;

	len = 0;
	var = env->yytext;
	if (!ft_isalpha(*var))
		return (0);
	while (ft_isalnum(*var))
	{
		var++;
		len++;
	}
	if (*var == '=')
	{
		env->yyleng = len;
		return (1);
	}
	return (0);
}

void	word(t_env *env, t_curr_items_ptrs *ptrs)
{
	char		*word;
	char		*temp;
	t_cmd		*curr_cmd;
	char		*key;

	curr_cmd = ptrs->curr_pipelst->u_item.cmd;
	if (curr_cmd->argv_top == -1 && verify_var_name(env))
	{
		key = ft_substr(env->yytext, 0, env->yyleng);
		if (!key)
		{
			env->error_func_name = "malloc";
			return ;
		}
		env->state |= PARSE_VAR;
		advance(env, 1);
		advance(env, 1);
		word = substring(env);
		if (!word)
		{
			free(word);
			env->error_custom_msg = SYNTAX_ERR;
			return ;
		}
		set(key, word, &(env->env_vars), env);
		env->state &= ~PARSE_VAR;
		return ;
	}
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
		|| match(SINGLE_QUOTE, env) || match(DOLLAR_SIGN, env))
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

void	item(t_env *env, t_curr_items_ptrs *ptrs)
{
	if (match(LP, env))
	{
		env->opened_parens++;
		advance(env, 1);
		ptrs->curr_pipelst->u_item.script = statements(env);
		if (env->error_custom_msg || env->error_func_name)
			return ;
		ptrs->curr_pipelst->type = NEXT_SCRIPT;
		return ;
	}
	ptrs->curr_pipelst->u_item.cmd = init_cmd(env);
	if (env->error_func_name)
		return ;
	ptrs->curr_pipelst->type = NEXT_PIPELST;
	word(env, ptrs);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	while (match(SPACE, env))
	{
		advance(env, 1);
		if (legal_lookahead(env, SUBSTRING, SINGLE_QUOTE, DOUBLE_QUOTE,
				DOLLAR_SIGN, HEREDOC, IN_FILE, OUT_FILE, APPEND_FILE,
				NULL_TOKEN))
			word(env, ptrs);
		if (env->error_custom_msg || env->error_func_name)
			return ;
	}
}

void	verify_pipelist(t_env *env, t_pipelist *pipelist)
{
	t_cmd	*curr_cmd;

	while (pipelist != NULL)
	{
		if (pipelist->type == NEXT_PIPELST)
		{
			curr_cmd = pipelist->u_item.cmd;
			if (!pipelist->pos && (curr_cmd->out_filename
					|| curr_cmd->in_filename
					|| curr_cmd->heredocs_top != -1))
				env->error_custom_msg = SYNTAX_ERR;
			else if ((pipelist->pos & HEAD)
				&& !(pipelist->pos & TAIL) && curr_cmd->out_filename)
				env->error_custom_msg = SYNTAX_ERR;
			else if (!(pipelist->pos & HEAD)
				&& pipelist->pos & TAIL && (curr_cmd->in_filename
					|| curr_cmd->heredocs_top != -1))
				env->error_custom_msg = SYNTAX_ERR;
			if (env->error_custom_msg)
				return ;
		}
		pipelist = pipelist->next;
	}
}

void	command(t_env *env, t_curr_items_ptrs *ptrs)
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
	curr_cmd_table->pipelist->pos = HEAD;
	curr_cmd_table->pipelist->next = NULL;
	ptrs->curr_pipelst = curr_cmd_table->pipelist;
	item(env, ptrs);
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
		ptrs->curr_pipelst->pos = 0;
		ptrs->curr_pipelst->next = NULL;
		advance(env, 1);
		item(env, ptrs);
		if (env->error_custom_msg || env->error_func_name)
			return ;
	}
	ptrs->curr_pipelst->pos |= TAIL;
	verify_pipelist(env, curr_cmd_table->pipelist);
}

void	expression(t_env *env, t_curr_items_ptrs *ptrs)
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
	command(env, ptrs);
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
		command(env, ptrs);
		if (env->error_custom_msg || env->error_func_name)
			return ;
	}
}

int	init_next_script(t_scripts **ptr, t_env *env)
{
	if (match(EOI, env) || match(NEWLINE, env))
		return (0);
	if (legal_lookahead(env, LP, IN_FILE, OUT_FILE,
			HEREDOC, APPEND_FILE, SINGLE_QUOTE,
			DOUBLE_QUOTE, DOLLAR_SIGN, SUBSTRING,
			VAR, NULL_TOKEN))
	{
		*ptr = (t_scripts *) malloc(sizeof(t_scripts));
		if (!(*ptr))
		{
			set_err_func_name(env, "malloc");
			return (0);
		}
		(*ptr)->next = NULL;
	}
	else
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (0);
	}
	return (1);
}

t_scripts	*statements(t_env *env)
{
	t_scripts			*scripts;
	t_curr_items_ptrs	ptrs;

	if (init_next_script(&scripts, env))
		ptrs.curr_script = scripts;
	else
		return (NULL);
	while (1)
	{
		expression(env, &ptrs);
		if (legal_lookahead(env, EOI, NEWLINE, NULL_TOKEN)
			&& env->opened_parens > 0)
		{
			env->error_custom_msg = SYNTAX_ERR;
			break ;
		}
		if (env->error_custom_msg || env->error_func_name)
			break ;
		if (match(RP, env) && env->opened_parens > 0)
		{
			env->opened_parens--;
			advance(env, 1);
			break ;
		}
		if ((match(RP, env) && !env->opened_parens)
			|| !legal_lookahead(env, SEMI, NEWLINE,
				EOI, NULL_TOKEN))
		{
			env->error_custom_msg = SYNTAX_ERR;
			break ;
		}
		if (legal_lookahead(env, EOI, NEWLINE, NULL_TOKEN))
			break ;
		if (match(SEMI, env))
		{
			advance(env, 1);
			if (!init_next_script(&(ptrs.curr_script->next), env))
				break ;
			else
				ptrs.curr_script = ptrs.curr_script->next;
		}
	}
	return (scripts);
}