#ifndef BUILTINS_H
# define BUILTINS_H

struct	s_env;
struct	s_env_vars;
struct	s_cmd;

void	check_builtins(struct s_cmd *cmd, struct s_env *env);
void	cd(struct s_cmd *cmd, struct s_env *env);
void	pwd(void);
void	ft_env(struct s_env_vars *vars);
void	ft_exit(struct s_env *env);

#endif
