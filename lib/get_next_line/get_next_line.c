/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 14:21:29 by                   #+#    #+#             */
/*   Updated: 2022/04/07 19:43:52 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/get_next_line.h"
#include "../../include/env_state.h"
#include <stdlib.h>
#include "../../include/libft.h"
#include "../../include/error.h"

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 333
#endif

int	ft_strchr_alt(const char *s, int c);

char	*get_curr_line(char **curr_lines, size_t new_line_len)
{
	char	*curr_n;
	char	*tmp;

	curr_n = (char *)malloc(sizeof(char) * new_line_len);
	if (!curr_n)
		return (curr_n);
	ft_strlcpy(curr_n, *curr_lines, new_line_len);
	tmp = ft_strdup(*curr_lines + new_line_len - 1);
	free(*curr_lines);
	*curr_lines = tmp;
	if (!*curr_lines)
	{
		free(curr_n);
		return (curr_n = NULL);
	}
	return (curr_n);
}

char	*get_next_line_from_state(char **curr_lines)
{
	char			*tmp;
	size_t			new_line_len;
	int				n_idx;

	n_idx = ft_strchr_alt(*curr_lines, '\n');
	if (n_idx == -1 && **curr_lines)
	{
		tmp = *curr_lines;
		*curr_lines = NULL;
		return (tmp);
	}
	else if (!**curr_lines)
	{
		free(*curr_lines);
		return (*curr_lines = NULL);
	}
	new_line_len = n_idx + 2;
	return (get_curr_line(curr_lines, new_line_len));
}

int	read_into_buffer(int fd, ssize_t *num_read, char **curr_lines)
{
	char	*tmp;
	char	buffer[BUFFER_SIZE + 1];

	*num_read = read(fd, buffer, BUFFER_SIZE);
	if (*num_read <= 0)
		return (0);
	buffer[*num_read] = '\0';
	tmp = ft_strjoin(*curr_lines, buffer);
	free(*curr_lines);
	*curr_lines = tmp;
	return (1);
}

char	*get_next_line(t_env *env, int fd)
{
	static char	*curr_lines = NULL;
	ssize_t		num_read;

	num_read = 0;
	if (!curr_lines)
	{
		curr_lines = (char *)malloc(sizeof(char));
		if (!curr_lines)
		{
			set_err_func_name(env, "malloc");
			return (curr_lines);
		}
		*curr_lines = '\0';
	}
	while (ft_strchr_alt(curr_lines, '\n') == -1
		&& read_into_buffer(fd, &num_read, &curr_lines))
	{
		if (!curr_lines)
		{
			set_err_func_name(env, "malloc");
			return (curr_lines);
		}
	}
	if (num_read < 0)
	{
		free(curr_lines);
		set_err_func_name(env, "read");
		return (curr_lines = NULL);
	}
	return (get_next_line_from_state(&curr_lines));
}
