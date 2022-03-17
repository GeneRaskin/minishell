#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_CMD 100
# define MAX_VARS 1000
# define MAX_ARGV 1000
# define MAX_HEREDOCS 100
# define SHELL_NAME "gene_shell$ "

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft/libft.h"
# include "lex.h"
# include <fcntl.h>
# include <stdarg.h>
# include "get_next_line/get_next_line.h"

extern char					**environ;
typedef struct s_scripts	t_scripts;

typedef struct s_cmd
{
	char		*argv[MAX_ARGV];
	int			argv_top;
	int			heredocs_top;
	char		*in_filename;
	char		*out_filename;
	char		*delimeters[MAX_HEREDOCS];
	int			append_mode;
	char		*output;
}	t_cmd;

# define NEXT_PIPELST 0x1
# define NEXT_SCRIPT 0x2

typedef struct s_pipelist
{
	union {
		t_cmd		*cmd;
		t_scripts	*script;
	}	u_item;
	int					type;
	struct s_pipelist	*next;
}	t_pipelist;

typedef struct s_cmd_table
{
	int			top;
	int			logical_op[MAX_CMD];
	t_pipelist	*cmd_arr[MAX_CMD];
}	t_cmd_table;

typedef struct s_env
{
	char		*yytext;
	int			yyleng;
	char		*env_vars[MAX_VARS];
	int			lookahead;
	int			state;
	int			exit_code;
}	t_env;

typedef struct s_scripts
{
	t_cmd_table			*cmd_table;
	struct s_scripts	*next;
	t_pipelist			*curr_pipelst;
	t_scripts			*curr_script;
}	t_scripts;

# include "executor.h"

/*typedef struct s_statement
{
	t_scripts	*scripts;
	t_pipelist	*curr_pipelst;
	t_scripts	*curr_script;
	int			in_fd;
	int			out_fd;
}	t_statement;*/

void		error(const char *func_name);
void		advance(t_env *env, int skip_spaces);
int			match(int token, t_env *env);
t_scripts	*statements(t_env *env);
void		close_descriptors(int num_fd, ...);
int			ft_isspace(char c);

#endif
