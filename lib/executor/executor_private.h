/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_private.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 01:34:50 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 01:38:28 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_PRIVATE_H
# define EXECUTOR_PRIVATE_H

struct	s_cmd;
struct	s_env;
struct	s_pipelist;
struct	s_env_vars;

typedef struct s_params
{
	int	first_in;
	int	*second_p;
	int	global_out;
	int	status;
}	t_params;

void	read_heredoc(struct s_cmd *cmd, struct s_env *env, int write_pipe);
int		set_input_fd(struct s_cmd *cmd, struct s_env *env, int custom_in);
int		set_output_fd(struct s_cmd *cmd, struct s_env *env, int custom_out);
void	close_descriptors(int num_fd, ...);
void	iter_pipelist(struct s_pipelist *curr_pipelist, struct s_env *env,
			int global_in, int global_out);
void	find_and_exec_cmd(struct s_cmd *cmd, struct s_env *env);
void	builtin(struct s_pipelist *curr_pipelist, struct s_env *env,
			t_params *params);
void	fork_process(struct s_pipelist *curr_pipelist, struct s_env *env,
			t_params *params);
int		first_block_pipelist(struct s_pipelist **curr_pipelist,
			struct s_env *env, t_params *params);
void	loop_pipelist(struct s_pipelist **curr_pipelist, struct s_env *env,
			t_params *params, int *first_p);
void	last_block_pipelist(struct s_pipelist *curr_pipelist, struct s_env *env,
			t_params *params);
int		catch_and_wait(struct s_env *env, int *status);
int		report_func_error(struct s_env *env, char *func_name);
int		init_pipe(struct s_pipelist *curr_pipelist,
			struct s_env *env, int *second_p);
int		construct_envp_loop(struct s_env_vars **globals, char **envp,
			int *len);

#endif
