/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 18:39:53 by                   #+#    #+#             */
/*   Updated: 2022/04/09 01:21:53 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	slen;

	i = start;
	if (s == NULL)
		return (NULL);
	slen = ft_strlen(s);
	if (start < slen && slen < len)
		substr = (char *) malloc(sizeof(char) * (slen - start + 1));
	else if (start < slen && slen >= len)
		substr = (char *) malloc(sizeof(char) * (len + 1));
	else
		substr = (char *) malloc(sizeof(char));
	if (substr == NULL)
		return (substr);
	if (start >= slen)
		*substr = '\0';
	else
	{
		while (i - start < len && s[i++])
			substr[i - 1 - start] = s[i - 1];
		substr[i - start] = '\0';
	}
	return (substr);
}
