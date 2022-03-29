#include "minishell.h"

void	init_env(t_env *env)
{
	env->lookahead = -1;
	env->yytext = "";
	env->yyleng = 0;
	env->state = 0;
	env->exit_code = -1;
}

int	main(void)
{
	t_env		env;
	t_scripts	*parse_tree;
	pid_t		curr_process;
	int			status;

	while (1)
	{
		init_env(&env);
		ft_putstr_fd(SHELL_NAME, STDOUT_FILENO);
		env.yytext = get_next_line(STDIN_FILENO);
		if (env.yytext == NULL)
			continue ;
		while (ft_isspace(*(env.yytext)))
			env.yytext++;
		parse_tree = statements(&env);
		if (parse_tree)
		{
			curr_process = fork();
			if (!curr_process)
				executor(parse_tree, &env, STDIN_FILENO, STDOUT_FILENO);
		}
		while (wait(&status) > 0)
			;
		free_parse_tree(parse_tree);
	}
}
