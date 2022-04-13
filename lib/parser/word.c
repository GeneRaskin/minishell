#include "parser_private.h"
#include "../../include/libft.h"
#include "../../include/env_vars.h"

char	*substring(t_env *env);
void	word(t_env *env, t_curr_items_ptrs *ptrs);

static int	verify_var_name(t_env *env)
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

static void	init_env_var(t_env *env)
{
	char	*key;
	char	*word;

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
}

static void	word_main_loop(t_env *env, char **word_ptr)
{
	char		*temp;

	*word_ptr = substring(env);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	while (match(SUBSTRING, env) || match(DOUBLE_QUOTE, env)
		|| match(SINGLE_QUOTE, env) || match(DOLLAR_SIGN, env))
	{
		temp = *word_ptr;
		*word_ptr = substring(env);
		if (env->error_custom_msg || env->error_func_name)
			return ;
		*word_ptr = ft_strjoin(temp, *word_ptr);
		if (!*word_ptr)
		{
			free(temp);
			set_err_func_name(env, "malloc");
			return ;
		}
		free(temp);
	}
}

static void	set_inputs_outputs(t_env *env, t_cmd *curr_cmd)
{
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
}

void	word(t_env *env, t_curr_items_ptrs *ptrs)
{
	char		*word;
	t_cmd		*curr_cmd;

	curr_cmd = ptrs->curr_pipelst->u_item.cmd;
	if (curr_cmd->argv_top == -1 && verify_var_name(env))
	{
		init_env_var(env);
		return ;
	}
	set_inputs_outputs(env, curr_cmd);
	word_main_loop(env, &word);
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
