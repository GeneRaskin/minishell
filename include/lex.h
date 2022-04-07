#ifndef LEX_H
# define LEX_H

struct	s_env;

void	advance(struct	s_env *env, int skip_spaces);
int		match(int token, struct	s_env *env);

#endif
