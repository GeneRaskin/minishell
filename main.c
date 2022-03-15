#include "minishell.h"

int	main(void)
{
	t_env		env;
	t_scripts	*parse_tree;

	env.lookahead = -1;
	env.yytext = "";
	env.yyleng = 0;
	env.state = 0;
	env.exit_code = -1;
	parse_tree = statements(&env);
	executor(parse_tree, &env, STDIN_FILENO, STDOUT_FILENO);
}
