/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_private.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 02:51:40 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/14 02:54:36 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_PRIVATE_H
# define PARSER_PRIVATE_H

# include "../../include/parse_tree.h"
# include "../../include/error.h"
# include "../../include/tokens.h"
# include "../../include/lex.h"
# include "../../include/env_state.h"
# include "../../include/libft.h"
# include "../../include/env_vars.h"
# include <stdlib.h>

typedef struct s_curr_items_ptrs
{
	t_pipelist	*curr_pipelst;
	t_cmd_table	*curr_cmd_table;
	t_scripts	*curr_script;
}	t_curr_items_ptrs;

void	set_inputs_outputs(t_env *env, t_cmd *curr_cmd);
void	word_main_loop(t_env *env, char **word_ptr);
char	*substring(t_env *env);
void	word(t_env *env, t_curr_items_ptrs *ptrs);
void	append_to_word(t_env *env, int in_global_vars,
			char **word, char *key);
int		init_key_vars(int *in_global_vars, char **key,
			t_env *env);
int		verify_var_name(t_env *env);

#endif
