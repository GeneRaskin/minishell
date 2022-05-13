/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_private.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lemmon <lemmon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 22:19:15 by lemmon            #+#    #+#             */
/*   Updated: 2022/05/13 22:19:15 by lemmon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_PRIVATE_H
# define PARSER_PRIVATE_H

# include "../../include/parse_tree.h"
# include "../../include/error.h"
# include "../../include/tokens.h"
# include "../../include/lex.h"
# include "../../include/env_state.h"
# include <stdlib.h>

typedef struct s_curr_items_ptrs
{
	t_pipelist	*curr_pipelst;
	t_cmd_table	*curr_cmd_table;
	t_scripts	*curr_script;
}	t_curr_items_ptrs;

#endif
