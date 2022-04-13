#include "include/error.h"
#include "include/libft.h"
#include "include/free.h"
#include "include/executor.h"
#include "include/parser.h"
#include "include/env_state.h"
#include "include/env_vars.h"
#include "include/parse_tree.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <signal.h>
#include "sigs.h"
#ifdef MALLOC_DEBUG
# include "malloc_debug.h"
extern int	g_malloc_count;
#endif
#define SHELL_NAME "gene_shell$ "

void	init_env(t_env *env)
{
	env->lookahead = -1;
	env->yytext = "";
	env->yyleng = 0;
	env->state = 0;
	env->exit_code = -1;
	env->opened_parens = 0;
	env->error_func_name = NULL;
	env->error_custom_msg = NULL;
	env->yytext = readline(SHELL_NAME);
}

void	execute(t_env *env, t_scripts *parse_tree)
{
	if (env->error_func_name || env->error_custom_msg)
	{
		error(env);
		free_parse_tree(parse_tree);
	}
	else if (parse_tree)
	{
		executor(parse_tree, env, STDIN_FILENO, STDOUT_FILENO);
		free_parse_tree(parse_tree);
	}
}

static void	loop(t_env *env)
{
	t_scripts		*parse_tree;
	char			*curr_line;

	while (1)
	{
		init_env(env);
		if (env->yytext && *(env->yytext))
			add_history(env->yytext);
		if (!env->yytext)
		{
			ft_putstr_fd("\n", STDOUT_FILENO);
			free_env_vars(env->env_vars);
			exit(EXIT_SUCCESS);
		}
		else if (!*(env->yytext))
			continue ;
		if (env->error_func_name)
		{
			error(env);
			continue ;
		}
		curr_line = env->yytext;
		while (ft_isspace(*(env->yytext)))
			env->yytext++;
		parse_tree = statements(env);
		free(curr_line);
		execute(env, parse_tree);
#ifdef MALLOC_DEBUG
		printf("malloc counter: %d\n", g_malloc_count);
#endif
	}
}

int	main(void)
{
	t_env	env;
#ifdef MALLOC_DEBUG
	set_zone();
#endif
	env.env_vars = NULL;
	signal(SIGINT, catch_sigint);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	loop(&env);
}
