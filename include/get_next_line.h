/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 14:23:07 by                   #+#    #+#             */
/*   Updated: 2022/04/07 02:09:21 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "global.h"
# include "aux.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 333
# endif

char	*get_next_line(t_env *env, int fd);
int		ft_strchr_alt(const char *s, int c);

#endif
