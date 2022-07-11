/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:41:11 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:42:25 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

struct	s_env;
struct	s_scripts;

struct s_scripts	*statements(struct s_env *env);
char				*dollar_expansion(struct s_env *env);

#endif
