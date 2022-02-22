#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_CMD 100
# define MAX_VARS 1000
# define MAX_ARGV 1000
# define MAX_INFILES 100
# define MAX_HEREDOCS 100

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
	int		in_files_top;
	int		heredocs_top;
	char	*in_filenames[MAX_INFILES];
	char	*out_filename;
	char	*delimeters[MAX_HEREDOCS];
	int		append_mode;
	int		exit_code;
	char	*output;
}	t_cmd;

typedef struct s_cmd_table
{
	int		top;
	t_cmd	*cmd_arr[MAX_CMD];
}	t_cmd_table;

typedef struct s_pipelist
{
	t_cmd_table			*cmd_table;
	struct s_pipelist	*next;
}	t_pipelist;

typedef struct s_scripts
{
	t_pipelist			*pipelist;
	struct s_scripts	*next;
}	t_scripts;

typedef struct s_env
{
	char		*yytext;
	int			yyleng;
	t_scripts	*scripts;
	char		*env_vars[MAX_VARS];
	int			lookahead;
	int			state;
	t_pipelist	*curr_pipelst;
	t_scripts	*curr_script;
}	t_env;

void	error(const char *func_name);
void	advance(t_env *env, int skip_spaces);
int		match(int token, t_env *env);
void	statements(t_env *env);

#endif
