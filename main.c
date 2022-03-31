#include "minishell.h"

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
	env->yytext = get_next_line(env, STDIN_FILENO);
}

void	execute(t_env *env, t_scripts *parse_tree)
{
	int		status;
	pid_t	curr_process;

	if (env->error_func_name || env->error_custom_msg)
	{
		error(env);
		free_parse_tree(parse_tree);
	}
	else
	{
		if (parse_tree)
		{
			curr_process = fork();
			if (!curr_process)
				executor(parse_tree, env, STDIN_FILENO, STDOUT_FILENO);
		}
		while (wait(&status) > 0)
			;
		free_parse_tree(parse_tree);
	}
}

int	main(void)
{
	t_env		env;
	t_scripts	*parse_tree;

	while (1)
	{
		ft_putstr_fd(SHELL_NAME, STDOUT_FILENO);
		init_env(&env);
		if (env.error_func_name)
		{
			error(&env);
			continue ;
		}
		if (env.yytext == NULL)
			continue ;
		while (ft_isspace(*(env.yytext)))
			env.yytext++;
		parse_tree = statements(&env);
		execute(&env, parse_tree);
	}
}
