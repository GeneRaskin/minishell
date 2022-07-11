/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:40:51 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:42:25 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

struct	s_scripts;

void		free_parse_tree(struct s_scripts *parse_tree);
void		free_2d_arr(void **arr);

#endif
