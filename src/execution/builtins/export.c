/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/17 14:18:10 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 13:58:45 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!key)
		return (false);
	if (!ft_isalpha(key[i]) && key[i] != '_')
	{
		return (false);
	}
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
		{
			return (false);
		}
		i++;
	}
	return (true);
}

static void	add_variable(t_list **head, t_list *new, t_list *cur, t_list *last)
{
	if (cur)
	{
		if (cur == *head)
		{
			new->next = cur->next;
			*head = new;
		}
		else
		{
			last->next = new;
			new->next = cur->next;
		}
		free_env_var((t_env_var *)(cur->as_ptr));
		free(cur);
	}
	else
		ft_lstadd_back(head, new);
}

static void	add_to_lst(t_list **head, t_list *new)
{
	t_list		*cur;
	t_list		*last;

	cur = *head;
	last = NULL;
	while (cur)
	{
		if (ft_strncmp(key(new), key(cur), ft_strlen(key(cur)) + 1) == 0)
			break ;
		last = cur;
		cur = cur->next;
	}
	add_variable(head, new, cur, last);
}

static t_error	export_arg(t_list **head, char *arg)
{
	t_list		*new;
	t_env_var	*env_var;

	env_var = create_env_var(arg);
	if (!env_var)
	{
		return (DEADLY_ERR);
	}
	if (!is_valid_key(env_var->key))
	{
		if (ft_strlen(env_var->key) > 0)
			ft_printf_fd(STDERR_FILENO, "minishell: export: `%s': \
not a valid identifier\n", env_var->key);
		else
			ft_printf_fd(STDERR_FILENO, "minishell: export: `=': \
not a valid identifier\n");
		set_exit_code(1);
		free_env_var(env_var);
		return (ERR);
	}
	new = ft_lstnew(env_var);
	if (!new)
		return (free_env_var(env_var), perror("malloc"), DEADLY_ERR);
	add_to_lst(head, new);
	return (NO_ERR);
}

t_error	exec_export(char **args)
{
	t_shell	*shell;
	t_error	error;
	size_t	i;

	shell = get_shell_struct();
	if (args[0] && args[1])
	{
		i = 1;
		while (args[i])
		{
			error = export_arg(&shell->env_lst, args[i]);
			if (error != NO_ERR)
			{
				return (error);
			}
			i++;
		}
	}
	else
	{
		print_ordered_lst();
	}
	return (NO_ERR);
}
