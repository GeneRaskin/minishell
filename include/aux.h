#ifndef AUX_H
# define AUX_H

# include "global.h"
# include "parse_tree.h"
# include "lex.h"

void		error(t_env *env);
void		set_err_custom_msg(t_env *env, char *str);
void		set_err_func_name(t_env *env, char *str);
int			legal_lookahead(t_env *env, ...);
void		free_parse_tree(t_scripts *parse_tree);
void		close_descriptors(int num_fd, ...);
void		free_2d_arr(void **arr);
int			ft_isspace(char c);

#endif
