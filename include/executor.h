/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:18 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:18 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

extern char	**environ;
struct		s_scripts;
struct		s_env;

void	executor(struct s_scripts *parse_tree, struct s_env *env,
			int global_in, int global_out);

#endif