/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 23:11:06 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/13 23:11:09 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/executor.h"
#include "../../include/tokens.h"
#include "../../include/parse_tree.h"
#include "../../include/env_state.h"
#include "../../include/libft.h"
#include "../../include/error.h"
#include "executor_private.h"

void	iter_cmd_table(t_cmd_table *curr_cmd_table, t_env *env,
						int global_in, int global_out)
{
	while (curr_cmd_table != NULL)
	{
		if ((curr_cmd_table->logical_op == OR && env->exit_code == EXIT_SUCCESS)
			|| (curr_cmd_table->logical_op == AND
				&& env->exit_code != EXIT_SUCCESS))
			curr_cmd_table = curr_cmd_table->next;
		else
		{
			iter_pipelist(curr_cmd_table->pipelist, env,
				global_in, global_out);
			curr_cmd_table = curr_cmd_table->next;
		}
	}
}

void	executor(t_scripts *parse_tree, t_env *env,
					int global_in, int global_out)
{
	while (parse_tree != NULL)
	{
		iter_cmd_table(parse_tree->cmd_table, env, global_in, global_out);
		parse_tree = parse_tree->next;
	}
}
