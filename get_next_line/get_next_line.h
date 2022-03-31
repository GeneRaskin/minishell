/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 14:23:07 by                   #+#    #+#             */
/*   Updated: 2021/11/04 00:46:51 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include "../minishell.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 333
# endif

char	*get_next_line(t_env *env, int fd);
int		ft_strchr_alt(const char *s, int c);

#endif
