/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_state.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:06:12 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:06:12 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_STATE_H
# define ENV_STATE_H

struct	s_scripts;

typedef struct s_env_vars
{
	char				*name;
	char				*value;
	struct s_env_vars	*next;
}	t_env_vars;

typedef struct s_env
{
	char				*yytext;
	int					yyleng;
	t_env_vars			*env_vars;
	t_env_vars			*global_env_vars;
	struct s_scripts	*parse_tree;
	int					lookahead;
	int					state;
	int					exit_code;
	char				*error_func_name;
	char				*error_custom_msg;
	int					opened_parens;
}	t_env;

#endif