/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:18:07 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:18:37 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"

void	item(t_env *env, t_curr_items_ptrs *ptrs);

static void	check_match(t_env *env, t_curr_items_ptrs *ptrs)
{
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
		item(env, ptrs);
		if (env->error_custom_msg || env->error_func_name)
			return ;
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
	curr_cmd_table->pipelist->next = NULL;
	ptrs->curr_pipelst = curr_cmd_table->pipelist;
	item(env, ptrs);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	check_match(env, ptrs);
}
