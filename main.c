#include "minishell.h"

int	main(void)
{
	t_env	env;

	env.lookahead = -1;
	env.yytext = "";
	env.yyleng = 0;
	env.state = 0;
	env.cmd_table = (t_cmd_table *) malloc(sizeof(t_cmd_table));
	env.cmd_table->top = 0;
	statements(&env);
}
