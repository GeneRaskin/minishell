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

static void	init_env_var(t_env *env) {
	char *key;
	char *word;

	key = ft_substr(env->yytext, 0, env->yyleng);
	if (!key) {
		env->error_func_name = "malloc";
		return;
	}
	env->state |= PARSE_VAR;
	advance(env, 1);
	advance(env, 1);
	word = substring(env);
	if (env->error_custom_msg || env->error_func_name)
		return;
	if (!get(key, env->global_env_vars))
		set(key, word, &(env->env_vars), env);
	else
		set(key, word, &(env->global_env_vars), env);
	env->state &= ~PARSE_VAR;
}

static void	word_main_loop(t_env *env, char **word_ptr)
{
	char		*temp;
	char		*joined;

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
		joined = ft_strjoin(temp, *word_ptr);
		if (!joined)
		{
			free(temp);
			free(*word_ptr);
			set_err_func_name(env, "malloc");
			return ;
		}
		free(*word_ptr);
		*word_ptr = joined;
		free(temp);
	}
}

static void	set_inputs_outputs(t_env *env, t_cmd *curr_cmd)
{
	if (legal_lookahead(env, HEREDOC, APPEND_FILE,
			IN_FILE, OUT_FILE, NULL_TOKEN))
	{
		if (legal_lookahead(env, APPEND_FILE, OUT_FILE, NULL_TOKEN))
		{
			env->state |= OUT_FILE_TK;
			curr_cmd->out_filenames_top++;
		}
		if (match(APPEND_FILE, env)
			&& curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->append_mode[curr_cmd->out_filenames_top]
				= 1;
		else if (match(IN_FILE, env))
			env->state |= IN_FILE_TK;
		else if (match(OUT_FILE, env)
			&& curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->append_mode[curr_cmd->out_filenames_top]
				= 0;
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
	{
		if (curr_cmd->in_filenames_top < MAX_INFILENAMES - 1)
			curr_cmd->in_filename[++curr_cmd->in_filenames_top] = word;
		else
			set_err_custom_msg(env, INFILENAMES_OVERFLOW_ERROR);
		curr_cmd->last_input = 1;
	}
	else if (env->state & OUT_FILE_TK)
	{
		if (curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->out_filename[curr_cmd->out_filenames_top] = word;
		else
		{
			curr_cmd->out_filenames_top--;
			set_err_custom_msg(env, OUTFILENAMES_OVERFLOW_ERROR);
		}
	}
	else if (env->state & HEREDOC_TK)
	{
		if (curr_cmd->heredocs_top < MAX_HEREDOCS - 1)
			curr_cmd->delimeters[++curr_cmd->heredocs_top] = word;
		else
			set_err_custom_msg(env, HEREDOCS_OVERFLOW_ERROR);
		curr_cmd->last_input = 2;
	}
	else
	{
		if (curr_cmd->argv_top < MAX_ARGV - 2)
			curr_cmd->argv[++curr_cmd->argv_top] = word;
		else
			set_err_custom_msg(env, ARGV_OVERFLOW_ERROR);
	}
	curr_cmd->argv[curr_cmd->argv_top + 1] = NULL;
	env->state &= ~IN_FILE_TK;
	env->state &= ~OUT_FILE_TK;
	env->state &= ~HEREDOC_TK;
}
