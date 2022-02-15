#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_CMD 100
# define MAX_VARS 1000
# define MAX_ARGV 1000

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft/libft.h"
# include "lex.h"

typedef struct s_cmd
{
	char	*binname;
	char	*argv[MAX_ARGV];
	int		argv_top;
	char	*in_filename;
	char	*out_filename;
	char	*delimeter;
	int		append_mode;
}	t_cmd;

typedef struct s_cmd_table
{
	unsigned int	top;
	t_cmd			*cmd_arr[MAX_CMD];
}	t_cmd_table;

typedef struct s_env
{
	char		*yytext;
	int			yyleng;
	t_cmd_table	*cmd_table;
	char		*env_vars[MAX_VARS];
	int			lookahead;
	int			state;
}	t_env;

void	error(const char *func_name);
void	advance(t_env *env, int skip_spaces);
int		match(int token, t_env *env);
void	statements(t_env *env);

#endif
