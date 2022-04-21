#include "parser_private.h"

void	expression(t_env *env, t_curr_items_ptrs *ptrs);

static int	init_next_script(t_scripts **ptr, t_env *env)
{
	if (match(EOI, env) || match(NEWLINE, env))
		return (0);
	if (legal_lookahead(env, LP, IN_FILE, OUT_FILE,
			HEREDOC, APPEND_FILE, SINGLE_QUOTE,
			DOUBLE_QUOTE, DOLLAR_SIGN, SUBSTRING,
			VAR, NULL_TOKEN))
	{
		*ptr = (t_scripts *) malloc(sizeof(t_scripts));
		if (!(*ptr))
		{
			set_err_func_name(env, "malloc");
			return (0);
		}
		(*ptr)->next = NULL;
	}
	else
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (0);
	}
	return (1);
}

static int	syntax_checking(t_env *env)
{
	if (env->error_custom_msg || env->error_func_name)
		return (0);
	if (legal_lookahead(env, EOI, NEWLINE, NULL_TOKEN)
		&& env->opened_parens > 0)
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (0);
	}
	if (match(RP, env) && env->opened_parens > 0)
	{
		env->opened_parens--;
		advance(env, 1);
		return (0);
	}
	if ((match(RP, env) && !env->opened_parens)
		|| !legal_lookahead(env, SEMI, NEWLINE,
			EOI, NULL_TOKEN))
	{
		env->error_custom_msg = SYNTAX_ERR;
		return (0);
	}
	if (legal_lookahead(env, EOI, NEWLINE, NULL_TOKEN))
		return (0);
	return (1);
}

t_scripts	*statements(t_env *env)
{
	t_scripts			*scripts;
	t_curr_items_ptrs	ptrs;

	if (init_next_script(&scripts, env))
		ptrs.curr_script = scripts;
	else
		return (NULL);
	while (1)
	{
		expression(env, &ptrs);
		if (!syntax_checking(env))
			break ;
		if (match(SEMI, env))
		{
			advance(env, 1);
			if (!init_next_script(&(ptrs.curr_script->next), env))
				break ;
			else
				ptrs.curr_script = ptrs.curr_script->next;
		}
	}
	return (scripts);
}
