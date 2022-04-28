#include "../../include/parse_tree.h"
#include "executor_private.h"
#include <unistd.h>
#include "../../include/env_state.h"
#include "../../include/builtins.h"

static int	builtin_first_block(t_pipelist **curr_pipelist, t_env *env,
									t_params *params, int status)
{
	params->status = status;
	builtin(*curr_pipelist, env, params);
	if ((*curr_pipelist)->next == NULL)
		return (0);
	else
		*curr_pipelist = (*curr_pipelist)->next;
	return (1);
}

static int	fork_first_block(t_pipelist **curr_pipelist, t_env *env,
								t_params *params)
{
	pid_t	curr_process;
	int		status;

	curr_process = fork();
	if (curr_process == -1)
		report_func_error(env, "fork");
	if (!curr_process)
	{
		if ((*curr_pipelist)->next != NULL)
			close_descriptors(1, params->second_p[0]);
		fork_process((*curr_pipelist), env, params);
	}
	if ((*curr_pipelist)->next != NULL)
		*curr_pipelist = (*curr_pipelist)->next;
	else
		return (catch_and_wait(env, &status));
	return (1);
}

int	first_block_pipelist(t_pipelist **curr_pipelist, t_env *env,
							t_params *params)
{
	int		status;

	if (!init_pipe(*curr_pipelist, env, params->second_p))
		return (0);
	if ((*curr_pipelist)->type == NEXT_PIPELST && find_builtin(
			(*curr_pipelist)->u_item.cmd, &status))
	{
		if (!builtin_first_block(curr_pipelist, env, params, status))
			return (0);
	}
	else
		if (!fork_first_block(curr_pipelist, env, params))
			return (0);
	return (1);
}

void	loop_pipelist(t_pipelist **curr_pipelist, t_env *env,
						t_params *params, int *first_p)
{
	pid_t	curr_process;
	int		status;

	params->first_in = first_p[0];
	if ((*curr_pipelist)->type == NEXT_PIPELST && find_builtin(
			(*curr_pipelist)->u_item.cmd, &status))
	{
		params->status = status;
		builtin((*curr_pipelist), env, params);
	}
	else
	{
		curr_process = fork();
		if (curr_process == -1)
			report_func_error(env, "fork");
		if (!curr_process)
		{
			close_descriptors(2, first_p[1], params->second_p[0]);
			fork_process(*curr_pipelist, env, params);
		}
	}
	close_descriptors(2, first_p[0], first_p[1]);
	*curr_pipelist = (*curr_pipelist)->next;
}

void	last_block_pipelist(t_pipelist *curr_pipelist, t_env *env,
								t_params *params)
{
	int		status;
	pid_t	curr_process;

	params->first_in = params->second_p[0];
	if (curr_pipelist->type == NEXT_PIPELST && find_builtin(
			curr_pipelist->u_item.cmd, &status))
	{
		params->status = status;
		builtin(curr_pipelist, env, params);
	}
	else
	{
		curr_process = fork();
		if (curr_process == -1)
			report_func_error(env, "fork");
		if (!curr_process)
		{
			close_descriptors(1, params->second_p[1]);
			fork_process(curr_pipelist, env, params);
		}
	}
	close_descriptors(2, params->second_p[1], params->second_p[0]);
	catch_and_wait(env, &status);
}
