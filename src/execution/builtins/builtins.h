/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:29:17 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/16 09:43:32 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../../minishell.h"
# include <stdio.h>

typedef struct s_env_var
{
	char			*key; //before equal sign
	char			*value; //after equal sign
	bool			equal;
}	t_env_var;

typedef struct s_parse_env
{
	char	*buffer;
	size_t	buffer_len;
	size_t	cursor_pos;
}	t_parse_env;

char		*get_key(t_env_var *env_var, t_parse_env *env_parse);
char		*get_value(t_env_var *env_var, t_parse_env *env_parse);
t_env_var	*get_env_var(char *env);
t_list		*create_envlst(char **env);

#endif