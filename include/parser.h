#ifndef PARSER_H
# define PARSER_H

# include "global.h"
# include "parse_tree.h"
# include "aux.h"
# include "lex.h"

typedef struct s_curr_items_ptrs
{
	t_pipelist	*curr_pipelst;
	t_cmd_table	*curr_cmd_table;
	t_scripts	*curr_script;
}	t_curr_items_ptrs;

void		item(t_env *env, t_curr_items_ptrs *ptrs);
t_scripts	*statements(t_env *env);

#endif
