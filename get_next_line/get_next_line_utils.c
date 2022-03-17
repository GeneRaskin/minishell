/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/30 14:21:26 by                   #+#    #+#             */
/*   Updated: 2021/11/01 14:29:48 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr_alt(const char *s, int c)
{
	int	idx;
	int	i;

	idx = -1;
	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
		{
			idx = i;
			break ;
		}
		i++;
	}
	if (s[i] == c)
		idx = i;
	return (idx);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	joined_len;
	size_t	s1_len;
	size_t	s2_len;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	joined_len = s1_len + s2_len;
	joined = (char *)malloc(sizeof(char) * (joined_len + 1));
	if (joined == NULL)
		return (joined);
	ft_memcpy(joined, s1, s1_len);
	ft_memcpy(joined + s1_len, s2, s2_len);
	joined[joined_len] = '\0';
	return (joined);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*s)
	{
		len++;
		s++;
	}
	return (len);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	char		*d;
	const char	*s;
	size_t		n;

	d = dst;
	s = src;
	n = dstsize;
	if (n != 0)
	{
		while (--n > 0)
		{
			*d++ = *src++;
			if (*(src - 1) == '\0')
				break ;
		}
	}
	if (n == 0)
	{
		if (dstsize != 0)
			*d = '\0';
		while (*src++)
			;
	}
	return (src - s - 1);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	if (src == NULL && dst == NULL)
		return (dst);
	d = dst;
	s = src;
	while (n--)
		*d++ = *s++;
	return (dst);
}
