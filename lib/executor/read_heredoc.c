/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <evraskin@edu.hse.ru>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 00:19:17 by                   #+#    #+#             */
/*   Updated: 2022/07/11 14:31:58 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include "../../include/libft.h"
#include "../../include/parse_tree.h"
#include "../../include/env_state.h"
#include "executor_private.h"
#include <signal.h>

extern t_env	*g_env;

static int	write_to_pipe(int write_pipe, t_env *env, char *curr_line)
{
	if (write(write_pipe, curr_line, ft_strlen(curr_line)) == -1)
	{
		free(curr_line);
		report_func_error(env, "write");
		return (0);
	}
	if (write(write_pipe, "\n", 1) == -1)
	{
		free(curr_line);
		report_func_error(env, "write");
		return (0);
	}
	return (1);
}

void	sigint_heredoc(int signum)
{
	(void) signum;
	exit(EXIT_FAILURE);
}

void	read_heredoc(t_cmd *cmd, t_env *env, int write_pipe)
{
	char	*curr_line;
	int		curr_idx;

	if (!(g_env->state & 0x100))
		signal(SIGINT, sigint_heredoc);
	curr_line = readline("> ");
	curr_idx = 0;
	while (curr_line != NULL && !(env->state & 0x400))
	{
		if (ft_strncmp(curr_line, cmd->delimeters[curr_idx],
				ft_strlen(curr_line)) == 0
			&& ft_strlen(curr_line) == ft_strlen(cmd->delimeters[curr_idx]))
			curr_idx++;
		else
			if (!write_to_pipe(write_pipe, env, curr_line))
				return ;
		free(curr_line);
		if (curr_idx > cmd->heredocs_top)
			break ;
		curr_line = readline("> ");
	}
	close(write_pipe);
}
