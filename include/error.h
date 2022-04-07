#ifndef ERROR_H
# define ERROR_H

# define SYNTAX_ERR "gene_shell: Syntax error"
# define KEY_NOT_FOUND_ERR "gene_shell: variable not set"
# define BIN_NOT_FOUND_ERR "gene_shell: command not found"

struct	s_env;

void		error(struct s_env *env);
void		set_err_custom_msg(struct s_env *env, char *str);
void		set_err_func_name(struct s_env *env, char *str);
int			legal_lookahead(struct s_env *env, ...);

#endif
