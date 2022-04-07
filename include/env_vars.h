#ifndef ENV_VARS_H
# define ENV_VARS_H

typedef struct s_env_vars
{
	char				*name;
	char				*value;
	struct s_env_vars	*next;
}	t_env_vars;

#endif
