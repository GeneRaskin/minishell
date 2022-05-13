/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:32 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:32 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

struct	s_env;
struct	s_scripts;

struct s_scripts	*statements(struct s_env *env);
char				*dollar_expansion(struct s_env *env);

#endif
