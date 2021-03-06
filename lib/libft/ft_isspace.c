/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:36:42 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:36:43 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	ft_isspace(char c)
{
	return ((c == '\t') || (c == '\f')
		|| (c == '\v') || (c == '\r') || (c == ' '));
}
