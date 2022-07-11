/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_private.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eugeneraskin <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 15:12:33 by eugeneras         #+#    #+#             */
/*   Updated: 2022/05/16 15:12:34 by eugeneras        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_PRIVATE_H
# define BUILTINS_PRIVATE_H

# include "../../include/libft.h"
# include <stdio.h>
# include "../../include/env_state.h"
# include "../../include/error.h"
# include "../../include/parse_tree.h"
# include "../../include/env_vars.h"
# include "../../include/free.h"
# include <readline/history.h>
# include <sys/param.h>

extern t_env	*g_env;

#endif
