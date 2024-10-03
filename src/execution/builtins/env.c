/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/16 10:00:03 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/11 10:40:02 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_envlst(t_list *head, int order)
{
	t_list	*tmp;

	tmp = head;
	if (order == 4)
	{
		while (tmp != NULL)
		{
			if (!((t_env_var *)(tmp->as_ptr))->equal)
			{
				printf("declare -x %s\n", ((t_env_var *)(tmp->as_ptr))->key);
			}
			else
			{
				if (((t_env_var *)(tmp->as_ptr))->key)
					printf("declare -x %s", ((t_env_var *)(tmp->as_ptr))->key);
				if (((t_env_var *)(tmp->as_ptr))->equal)
					printf("=");
				if (((t_env_var *)(tmp->as_ptr))->value)
					printf("\"%s\"\n", ((t_env_var *)(tmp->as_ptr))->value);
			}
			tmp = tmp->next;
		}
	}
	else
	{
		while (tmp != NULL)
		{
			if (!((t_env_var *)(tmp->as_ptr))->equal)
			{
				tmp = tmp->next;
			}
			else
			{
				printf("%s", ((t_env_var *)(tmp->as_ptr))->key);
				if (((t_env_var *)(tmp->as_ptr))->equal)
					printf("=");
				printf("%s\n", ((t_env_var *)(tmp->as_ptr))->value);
				tmp = tmp->next;
			}
		}
	}
}

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

t_error	env(char **args)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (args[1])
		printf("Too many arguments, please type only env\n");
	else
		print_envlst(shell->env_lst, 6);
	set_exit_code(0);
	return (NO_ERR);
}

