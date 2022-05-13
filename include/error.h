/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:16 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:16 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# define SYNTAX_ERR "gene_shell: Syntax error"
# define KEY_NOT_FOUND_ERR "gene_shell: variable not set"
# define BIN_NOT_FOUND_ERR "gene_shell: command not found"
# define CD_PATH_ERROR "cd: path not found"
# define CD_ARGC_ERROR "cd: string not in pwd"
# define PWD_ARGC_ERROR "pwd: too many arguments"
# define ENV_ARGC_ERROR "env: too many arguments"
# define UNSET_ARGC_ERROR "unset: not enough arguments"
# define ARGV_OVERFLOW_ERROR "gene_shell: too many arguments in command"
# define INFILENAMES_OVERFLOW_ERROR "gene_shell: too many inputs in command"
# define OUTFILENAMES_OVERFLOW_ERROR "gene_shell: too many outputs in command"
# define HEREDOCS_OVERFLOW_ERROR "gene_shell: too many delimeters in command"

struct	s_env;

void		error(struct s_env *env);
void		set_err_custom_msg(struct s_env *env, char *str);
void		set_err_func_name(struct s_env *env, char *str);
int			legal_lookahead(struct s_env *env, ...);

#endif
