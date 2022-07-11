/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter_pipelist.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:35:28 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:35:29 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parse_tree.h"
#include "../../include/env_state.h"
#include "../../include/error.h"
#include "executor_private.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int	catch_and_wait(t_env *env, int *status)
{
	while (wait(status) > 0)
		;
	env->exit_code = WEXITSTATUS(*status);
	return (0);
}

int	report_func_error(t_env *env, char *func_name)
{
	set_err_func_name(env, func_name);
	error(env);
	env->exit_code = EXIT_FAILURE;
	return (0);
}

int	init_pipe(t_pipelist *curr_pipelist, t_env *env, int *second_p)
{
	if (curr_pipelist->type == NEXT_PIPELST
		&& curr_pipelist->u_item.cmd->argv_top == -1)
		return (0);
	if (curr_pipelist->next != NULL)
	{
		if (pipe(second_p) == -1)
			return (report_func_error(env, "pipe"));
	}
	return (1);
}

static int	dup_second_p(t_env *env, int *first_p, int *second_p)
{
	first_p[0] = dup(second_p[0]);
	if (first_p[0] == -1)
	{
		report_func_error(env, "dup");
		close_descriptors(2, second_p[0], second_p[1]);
		return (0);
	}
	first_p[1] = dup(second_p[1]);
	if (first_p[1] == -1)
	{
		report_func_error(env, "dup");
		close_descriptors(3, first_p[0], second_p[0], second_p[1]);
		return (0);
	}
	close_descriptors(2, second_p[0], second_p[1]);
	return (1);
}

void	iter_pipelist(t_pipelist *curr_pipelist, t_env *env,
			int global_in, int global_out)
{
	int			first_p[2];
	int			second_p[2];
	t_params	params;

	params.global_out = global_out;
	params.second_p = second_p;
	params.first_in = global_in;
	if (!first_block_pipelist(&curr_pipelist, env, &params))
		return ;
	while (curr_pipelist->next != NULL)
	{
		if (!dup_second_p(env, first_p, second_p))
			return ;
		if (pipe(second_p) == -1)
		{
			report_func_error(env, "pipe");
			return ;
		}
		params.second_p = second_p;
		loop_pipelist(&curr_pipelist, env, &params, first_p);
	}
	params.second_p = second_p;
	last_block_pipelist(curr_pipelist, env, &params);
}
