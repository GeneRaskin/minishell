#include "minishell.h"

int	main(void)
{
	t_env	env;

	env.lookahead = -1;
	env.yytext = "";
	env.yyleng = 0;
	env.state = 0;
	statements(&env);
}
