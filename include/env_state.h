/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_state.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:40:26 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:42:25 by                  ###   ########.fr       */
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
