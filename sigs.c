#include "sigs.h"
#include "include/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <signal.h>

void	catch_sigint(int signum)
{
	(void) signum;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	signal(SIGINT, catch_sigint);
}
