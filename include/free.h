/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:20 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:20 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FREE_H
# define FREE_H

struct	s_scripts;

void		free_parse_tree(struct s_scripts *parse_tree);
void		free_2d_arr(void **arr);

#endif
