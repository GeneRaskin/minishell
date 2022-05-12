#ifndef PARSE_TREE_H
# define PARSE_TREE_H

# define MAX_ARGV 100
# define MAX_HEREDOCS 100
# define MAX_INFILENAMES 100
# define MAX_OUTFILENAMES 100
# define NEXT_PIPELST 0x1
# define NEXT_SCRIPT 0x2

typedef struct s_cmd
{
	char	*argv[MAX_ARGV];
	int		argv_top;
	int		heredocs_top;
	int		in_filenames_top;
	int		out_filenames_top;
	char	*in_filename[MAX_INFILENAMES];
	char	*out_filename[MAX_OUTFILENAMES];
	char	*delimeters[MAX_HEREDOCS];
	int		append_mode[MAX_OUTFILENAMES];
	int		last_input;
}	t_cmd;

typedef struct s_pipelist
{
	union {
		t_cmd				*cmd;
		struct s_scripts	*script;
	}	u_item;
	int					type;
	struct s_pipelist	*next;
}	t_pipelist;

typedef struct s_cmd_table
{
	int					logical_op;
	t_pipelist			*pipelist;
	struct s_cmd_table	*next;
}	t_cmd_table;

typedef struct s_scripts
{
	t_cmd_table			*cmd_table;
	struct s_scripts	*next;
}	t_scripts;

#endif
