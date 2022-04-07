#ifndef GLOBAL_H
# define GLOBAL_H

# define SHELL_NAME "gene_shell$ "
# define SYNTAX_ERR "gene_shell: Syntax error"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdarg.h>
# include "env_vars.h"
# include "libft.h"

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
