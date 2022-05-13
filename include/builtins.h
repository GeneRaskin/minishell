/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:10 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:10 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

struct	s_env;
struct	s_env_vars;
struct	s_cmd;

# define CD 1
# define PWD 2
# define ENV 3
# define EXIT 4
# define UNSET 5
# define EXPORT 6
# define ECHO 7

void	call_builtins(struct s_cmd *cmd, struct s_env *env, int which_builtin);
int		find_builtin(struct s_cmd *cmd, int *which_builtin);
void	cd(struct s_cmd *cmd, struct s_env *env);
void	pwd(struct s_cmd *cmd, struct s_env *env);
void	ft_env(struct s_cmd *cmd, struct s_env *env);
void	ft_exit(struct s_cmd *cmd, struct s_env *env);
void	ft_unset(struct s_cmd *cmd, struct s_env *env);
void	echo(struct s_cmd *cmd, struct s_env *env);
void	export(struct s_cmd *cmd, struct s_env *env);

#endif
