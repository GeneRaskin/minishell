#ifndef ENV_VARS_H
# define ENV_VARS_H

struct	s_env_vars;
struct	s_env;

char		*get(char *key, struct s_env_vars *vars, struct s_env *env);
void		set(char *key, char *value, struct s_env_vars **vars,
				struct s_env *env);
void		free_env_vars(struct s_env_vars *vars);
void		unset(char *key, struct s_env_vars *env_vars, struct s_env *env);

#endif
