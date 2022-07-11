/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 20:13:14 by                   #+#    #+#             */
/*   Updated: 2022/05/14 02:13:30 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		i;
	size_t		needle_len;
	const char	*ptr;

	i = 0;
	ptr = NULL;
	needle_len = ft_strlen(needle);
	if (needle_len == 0)
		return ((char *)haystack);
	while (haystack[i] != '\0')
	{
		if (haystack[i] == needle[0])
		{
			if ((needle_len + i <= len)
				&& ft_strncmp(&haystack[i], needle, needle_len) == 0)
			{
				ptr = &haystack[i];
				break ;
			}
		}
		i++;
	}
	return ((char *)ptr);
}
