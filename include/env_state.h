#ifndef ENV_STATE_H
# define ENV_STATE_H

typedef struct s_env_vars
{
	char				*name;
	char				*value;
	struct s_env_vars	*next;
}	t_env_vars;

typedef struct s_env
{
	char		*yytext;
	int			yyleng;
	t_env_vars	*env_vars;
	int			lookahead;
	int			state;
	int			exit_code;
	char		*error_func_name;
	char		*error_custom_msg;
	int			opened_parens;
}	t_env;

#endif