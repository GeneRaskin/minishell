#include "minishell.h"

void	error(const char *func_name)
{
	perror(func_name);
	exit(EXIT_FAILURE);
}

int	legal_lookahead(int token, ...)
{

}
