#include "sigs.h"
#include "include/env_state.h"
#include "include/tokens.h"
#include "include/libft.h"
#include <signal.h>

extern t_env	g_env;

void	catch_sigint(int signum)
{
	(void) signum;
	g_env.state |= END_READLINE;
	signal(SIGINT, catch_sigint);
}
