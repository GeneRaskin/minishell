/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:14 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:14 by lemmon           ###   ########.fr       */
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
