/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/16 10:00:03 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/16 10:00:58 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_key(t_env_var *env_var, t_parse_env *env_parse)
{
	while (env_parse->buffer[env_parse->cursor_pos] != '=')
		env_parse->cursor_pos++;
	env_var->key = malloc((sizeof(char) * env_parse->cursor_pos) + 1);
	if (env_var->key == NULL)
	{
		free(env_var);
		return (NULL);
	}
	ft_strlcpy(env_var->key, env_parse->buffer, env_parse->cursor_pos + 1);
	return (env_var->key);
}

char	*get_value(t_env_var *env_var, t_parse_env *env_parse)
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
		free(env_var);
		return (NULL);
	}
	pos = env_parse->cursor_pos - i;
	ft_strlcpy(env_var->value, &env_parse->buffer[pos], i + 1);
	env_var->value = ft_strjoin(env_var->value, "\n");
	return (env_var->value);
}

t_env_var	*get_env_var(char *env)
{
	t_env_var	*env_var;
	t_parse_env	env_parse;

	env_parse.buffer = env;
	env_parse.cursor_pos = 0;
	env_parse.buffer_len = ft_strlen(env);

	env_var = malloc(sizeof(t_env_var));
	env_var->key = get_key(env_var, &env_parse);
	env_var->equal = (env[env_parse.cursor_pos] == '=');
	env_parse.cursor_pos++;
	env_var->value = get_value(env_var, &env_parse);

	return (env_var);

}

void	free_env_var(void *node)
{
	t_env_var	*env_var;

	env_var = (t_env_var *) node;
	free(env_var->key);
	free(env_var->value);
	free(env_var);
}

t_list	*create_envlst(char **env)
{
	t_env_var	*env_var;
	t_list		*head;
	t_list		*new;
	int			i;

	i = 0;
	head = NULL;
	while (env[i])
	{
		env_var = get_env_var(env[i]);
		if (!env_var)
		{
			ft_lstclear(&head, free_env_var);
			return (NULL);
		}
		new = ft_lstnew(env_var);
		if (!new)
		{
			ft_lstclear(&head, free_env_var);
			return (NULL);
		}
		ft_lstadd_back(&head, new);
		i++;
	}
	return (head);
}
