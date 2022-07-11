/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/14 02:51:29 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/14 02:51:31 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_private.h"

void	set_inputs_outputs(t_env *env, t_cmd *curr_cmd)
{
	if (legal_lookahead(env, HEREDOC, APPEND_FILE,
			IN_FILE, OUT_FILE, NULL_TOKEN))
	{
		if (legal_lookahead(env, APPEND_FILE, OUT_FILE, NULL_TOKEN))
		{
			env->state |= OUT_FILE_TK;
			curr_cmd->out_filenames_top++;
		}
		if (match(APPEND_FILE, env)
			&& curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->append_mode[curr_cmd->out_filenames_top]
				= 1;
		else if (match(IN_FILE, env))
			env->state |= IN_FILE_TK;
		else if (match(OUT_FILE, env)
			&& curr_cmd->out_filenames_top <= MAX_OUTFILENAMES - 1)
			curr_cmd->append_mode[curr_cmd->out_filenames_top]
				= 0;
		else if (match(HEREDOC, env))
			env->state |= HEREDOC_TK;
		advance(env, 1);
	}
}

void	word_main_loop(t_env *env, char **word_ptr)
{
	char	*temp;
	char	*joined;

	*word_ptr = substring(env);
	if (env->error_custom_msg || env->error_func_name)
		return ;
	while (match(SUBSTRING, env) || match(DOUBLE_QUOTE, env)
		|| match(SINGLE_QUOTE, env) || match(DOLLAR_SIGN, env))
	{
		temp = *word_ptr;
		*word_ptr = substring(env);
		if (env->error_custom_msg || env->error_func_name)
			return ;
		joined = ft_strjoin(temp, *word_ptr);
		if (!joined)
		{
			free(temp);
			free(*word_ptr);
			set_err_func_name(env, "malloc");
			return ;
		}
		free(*word_ptr);
		*word_ptr = joined;
		free(temp);
	}
}

int	verify_var_name(t_env *env)
{
	char	*var;
	int		len;

	len = 0;
	var = env->yytext;
	if (!ft_isalpha(*var))
		return (0);
	while (ft_isalnum(*var))
	{
		var++;
		len++;
	}
	if (*var == '+' && *(var + 1) == '=')
	{
		env->yyleng = len;
		env->state |= APPEND_TO_VAR;
		return (1);
	}
	if (*var == '=')
	{
		env->yyleng = len;
		return (1);
	}
	return (0);
}

void	append_to_word(t_env *env, int in_global_vars,
						char **word, char *key)
{
	char	*temp;

	env->state &= ~APPEND_TO_VAR;
	temp = *word;
	if (!in_global_vars)
	{
		*word = get(key, env->env_vars);
		if (*word)
		{
			*word = ft_strjoin(*word, temp);
			free(temp);
			return ;
		}
	}
	else
	{
		*word = get(key, env->global_env_vars);
		if (*word)
		{
			*word = ft_strjoin(*word, temp);
			free(temp);
			return ;
		}
	}
	*word = temp;
}

int	init_key_vars(int *in_global_vars, char **key,
							t_env *env)
{
	*key = ft_substr(env->yytext, 0, env->yyleng);
	*in_global_vars = 0;
	if (!*key)
	{
		env->error_func_name = "malloc";
		return (0);
	}
	env->state |= PARSE_VAR;
	advance(env, 1);
	if (match(PLUS_SIGN, env))
		advance(env, 1);
	advance(env, 1);
	return (1);
}
