/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:22 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:22 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEX_H
# define LEX_H

struct	s_env;

void	advance(struct	s_env *env, int skip_spaces);
int		match(int token, struct	s_env *env);

#endif
