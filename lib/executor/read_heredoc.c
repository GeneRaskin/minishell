/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <evraskin@edu.hse.ru>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 00:19:17 by                   #+#    #+#             */
/*   Updated: 2021/12/01 03:13:53 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include "../../include/libft.h"
#include "../../include/parse_tree.h"

void	read_heredoc(t_cmd *cmd, int write_pipe)
{
	char	*curr_line;
	int		curr_idx;

	curr_line = readline("> ");
	curr_idx = 0;
	while (curr_line != NULL)
	{
		if (ft_strncmp(curr_line, cmd->delimeters[curr_idx],
				ft_strlen(curr_line)) == 0)
			curr_idx++;
		else
		{
			write(write_pipe, curr_line, ft_strlen(curr_line));
			write(write_pipe, "\n", 1);
		}
		free(curr_line);
		if (curr_idx > cmd->heredocs_top)
			break ;
		curr_line = readline("> ");
	}
	close(write_pipe);
	//exit(EXIT_SUCCESS);
}
