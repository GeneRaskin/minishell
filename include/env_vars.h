/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:40:34 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:42:25 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VARS_H
# define ENV_VARS_H

struct	s_env_vars;
struct	s_env;

char		*get(char *key, struct s_env_vars *vars);
void		set(char *key, char *value, struct s_env_vars **vars,
				struct s_env *env);
void		free_env_vars(struct s_env_vars *vars);
void		unset(char *key, struct s_env_vars **env_vars);

#endif
