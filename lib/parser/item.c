#include "parser_private.h"

void		word(t_env *env, t_curr_items_ptrs *ptrs);
t_scripts	*statements(t_env *env);

static t_cmd	*init_cmd(t_env *env)
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

static int	init_word(t_env *env, t_curr_items_ptrs *ptrs)
{
	if (match(LP, env))
	{
		env->opened_parens++;
		advance(env, 1);
		ptrs->curr_pipelst->u_item.script = statements(env);
		if (env->error_custom_msg || env->error_func_name)
			return (0);
		ptrs->curr_pipelst->type = NEXT_SCRIPT;
		return (0);
	}
	ptrs->curr_pipelst->u_item.cmd = init_cmd(env);
	if (env->error_func_name)
		return (0);
	ptrs->curr_pipelst->type = NEXT_PIPELST;
	word(env, ptrs);
	if (env->error_custom_msg || env->error_func_name)
		return (0);
	return (1);
}

void	item(t_env *env, t_curr_items_ptrs *ptrs)
{
	if (!init_word(env, ptrs))
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
