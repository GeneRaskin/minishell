/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expression.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:19:10 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:19:10 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"

void	command(t_env *env, t_curr_items_ptrs *ptrs);

static int	init_cmd_table(t_env *env, t_curr_items_ptrs *ptrs)
{
	t_scripts		*curr_script;

	curr_script = ptrs->curr_script;
	curr_script->cmd_table = (t_cmd_table *) malloc(sizeof(t_cmd_table));
	if (!curr_script->cmd_table)
	{
		set_err_func_name(env, "malloc");
		return (0);
	}
	curr_script->cmd_table->next = NULL;
	ptrs->curr_cmd_table = curr_script->cmd_table;
	command(env, ptrs);
	if (env->error_custom_msg || env->error_func_name)
		return (0);
	ptrs->curr_cmd_table->logical_op = 0;
	return (1);
}

void	expression(t_env *env, t_curr_items_ptrs *ptrs)
{
	if (!init_cmd_table(env, ptrs))
		return ;
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
