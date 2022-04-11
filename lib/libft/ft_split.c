/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:24:46 by                   #+#    #+#             */
/*   Updated: 2022/04/09 01:21:53 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static size_t	ft_count_words(char const *s, char c)
{
	int	num_words;

	num_words = 0;
	while (*s)
	{
		if (*s != c)
		{
			num_words++;
			while (*s && *s != c)
				s++;
		}
		if (*s)
			s++;
	}
	return (num_words);
}

static char	*ft_create_word(char const **s, char c)
{
	size_t	word_size;
	size_t	i;
	char	*new_word;

	word_size = 0;
	i = 0;
	while ((*s)[word_size] && (*s)[word_size] != c)
		word_size++;
	new_word = (char *)malloc(sizeof(char) * (word_size + 1));
	if (new_word == NULL)
		return (new_word);
	while (i < word_size)
	{
		new_word[i] = *((*s)++);
		i++;
	}
	new_word[word_size] = '\0';
	return (new_word);
}

static void	*ft_free(char **s, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		free(s[i]);
		i++;
	}
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t	num_words;
	size_t	i;
	char	**splitted;

	if (s == NULL)
		return (NULL);
	num_words = ft_count_words(s, c);
	i = 0;
	splitted = (char **)malloc(sizeof(char *) * (num_words + 1));
	if (splitted == NULL)
		return (splitted);
	while (*s)
	{
		if (*s != c)
		{
			splitted[i] = ft_create_word(&s, c);
			if (splitted[i] == NULL)
				return (ft_free(splitted, i));
			i++;
		}
		if (*s)
			s++;
	}
	splitted[num_words] = NULL;
	return (splitted);
}
