#include "parser_private.h"

void	item(t_env *env, t_curr_items_ptrs *ptrs);

static void	verify_pipelist(t_env *env, t_pipelist *pipelist)
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
