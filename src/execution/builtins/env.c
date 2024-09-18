/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/16 10:00:03 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/18 13:48:36 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_key(t_env_var *env_var, t_parse_env *env_parse)
{
	while (env_parse->buffer[env_parse->cursor_pos] != '=')
	{
		if (env_parse->buffer[env_parse->cursor_pos] == '\0')
			break ;
		env_parse->cursor_pos++;
	}
	env_var->key = malloc((sizeof(char) * env_parse->cursor_pos) + 1);
	if (env_var->key == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(env_var->key, env_parse->buffer, env_parse->cursor_pos + 1);
	return (env_var->key);
}

static char	*get_value(t_env_var *env_var, t_parse_env *env_parse)
{
	int	i;
	int	pos;

	i = 0;
	while (env_parse->buffer[env_parse->cursor_pos] != '\0')
	{
		env_parse->cursor_pos++;
		i++;
	}
	env_var->value = malloc((sizeof(char) * i) + 2);
	if (env_var->value == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	pos = env_parse->cursor_pos - i;
	ft_strlcpy(env_var->value, &env_parse->buffer[pos], i + 1);
	return (env_var->value);
}

t_env_var	*create_env_var(char *env)
{
	t_env_var	*env_var;
	t_parse_env	env_parse;

	env_parse.buffer = env;
	env_parse.cursor_pos = 0;
	env_parse.buffer_len = ft_strlen(env);
	env_var = ft_calloc(1, sizeof(t_env_var));
	if (!env_var)
		return (perror("malloc"), NULL);
	env_var->key = get_key(env_var, &env_parse);
	if (!env_var->key)
		return (free(env_var), NULL);
	if (env[env_parse.cursor_pos] != '=')
		return (env_var);
	env_var->equal = (env[env_parse.cursor_pos] == '=');
	env_parse.cursor_pos++;
	env_var->value = get_value(env_var, &env_parse);
	if (!env_var->value)
	{
		free_env_var((void *) env_var);
		return (NULL);
	}
	return (env_var);
}

t_list	*create_envlst(char **env)
{
	t_env_var	*env_var;
	t_list		*head;
	t_list		*new;
	int			i;

	i = -1;
	head = NULL;
	while (env[++i])
	{
		env_var = create_env_var(env[i]);
		if (!env_var)
		{
			ft_lstclear(&head, free_env_var);
			return (NULL);
		}
		new = ft_lstnew(env_var);
		if (!new)
		{
			perror("malloc");
			ft_lstclear(&head, free_env_var);
			return (NULL);
		}
		ft_lstadd_back(&head, new);
	}
	return (head);
}
