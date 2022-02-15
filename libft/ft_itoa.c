/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <>                                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 19:29:48 by                   #+#    #+#             */
/*   Updated: 2021/10/07 20:50:30 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	ft_numlen(int n)
{
	unsigned int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n == INT_MIN)
	{
		len++;
		n = n % 1000000000;
	}
	if (n < 0)
	{
		len++;
		n *= (-1);
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static int	ft_mod(int n)
{
	if (n < 0)
		return ((-1) * (n % 10));
	return (n % 10);
}

char	*ft_itoa(int n)
{
	char			*num;
	unsigned int	num_len;
	unsigned int	min_pos;

	num_len = ft_numlen(n);
	num = (char *)malloc(sizeof(char) * (num_len + 1));
	if (num == NULL)
		return (num);
	num[num_len] = '\0';
	min_pos = 0;
	if (n == INT_MIN)
	{
		num[1] = '2';
		n = n % 1000000000;
		min_pos++;
	}
	if (n < 0 && ++min_pos)
		num[0] = '-';
	while (num_len > min_pos)
	{
		num[--num_len] = ft_mod(n) + '0';
		n /= 10;
	}
	return (num);
}
