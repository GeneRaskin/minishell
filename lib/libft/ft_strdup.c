/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 16:39:20 by                   #+#    #+#             */
/*   Updated: 2022/04/09 01:21:53 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s_copy;
	size_t	len;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	s_copy = (char *)malloc(sizeof(*s1) * (len + 1));
	if (s_copy == NULL)
	{
		errno = ENOMEM;
		return (s_copy);
	}
	ft_strlcpy(s_copy, s1, len + 1);
	return (s_copy);
}
