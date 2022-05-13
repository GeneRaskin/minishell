#include "executor_private.h"
#include "../../include/parse_tree.h"
#include <unistd.h>
#include "../../include/builtins.h"
#include "../../include/env_state.h"
#include <stdlib.h>
#include "../../include/executor.h"
#include "../../include/tokens.h"

static int	dup_stdio(t_env *env, int *standard_backup)
{
	standard_backup[0] = dup(STDIN_FILENO);
	if (standard_backup[0] == -1)
	{
		report_func_error(env, "dup");
		return (0);
	}
	standard_backup[1] = dup(STDOUT_FILENO);
	if (standard_backup[1] == -1)
	{
		report_func_error(env, "dup");
		close_descriptors(1, standard_backup[0]);
		return (0);
	}
	return (1);
}

void	builtin(t_pipelist *curr_pipelist, t_env *env,
					t_params *params)
{
	int		standard_backup[2];

	if (!dup_stdio(env, standard_backup))
		return ;
	if (!set_input_fd(curr_pipelist->u_item.cmd, env, params->first_in))
	{
		close_descriptors(2, standard_backup[0], standard_backup[1]);
		return ;
	}
	if (curr_pipelist->next != NULL)
	{
		if (!set_output_fd(curr_pipelist->u_item.cmd,
				env, params->second_p[1]))
			return ;
	}
	else
	{
		if (!set_output_fd(curr_pipelist->u_item.cmd,
				env, params->global_out))
			return ;
	}
	call_builtins(curr_pipelist->u_item.cmd, env, params->status);
	dup2(standard_backup[0], STDIN_FILENO);
	dup2(standard_backup[1], STDOUT_FILENO);
	close_descriptors(2, standard_backup[0], standard_backup[1]);
}

void	fork_process(t_pipelist *curr_pipelist, t_env *env,
							t_params *params)
{
	if (curr_pipelist->type == NEXT_PIPELST)
	{
		if (!set_input_fd(curr_pipelist->u_item.cmd, env, params->first_in))
			exit(EXIT_FAILURE);
		if (curr_pipelist->next != NULL)
			if (!set_output_fd(curr_pipelist->u_item.cmd, env,
					params->second_p[1]))
				exit(EXIT_FAILURE);
		if (!set_output_fd(curr_pipelist->u_item.cmd, env,
				params->global_out))
			exit(EXIT_FAILURE);
		find_and_exec_cmd(curr_pipelist->u_item.cmd, env);
	}
	else if (curr_pipelist->type == NEXT_SCRIPT)
	{
		if (curr_pipelist->next != NULL)
			executor(curr_pipelist->u_item.script, env, params->first_in,
				params->second_p[1]);
		else
			executor(curr_pipelist->u_item.script, env, params->first_in,
				params->global_out);
	}
	exit(EXIT_SUCCESS);
}
