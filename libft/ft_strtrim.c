/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 21:24:55 by                   #+#    #+#             */
/*   Updated: 2021/10/07 17:19:54 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *s2)
{
	size_t	remaining_len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	while (*s1 && ft_strchr(s2, *s1))
		s1++;
	remaining_len = ft_strlen(s1);
	while (remaining_len > 0 && ft_strchr(s2, s1[remaining_len]))
		remaining_len--;
	return (ft_substr(s1, 0, remaining_len + 1));
}
