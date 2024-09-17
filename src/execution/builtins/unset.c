/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/21 11:10:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/12 13:43:11 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	is_same_key(char *key, t_list *cur)
{
	return (ft_strncmp(key, ((t_env_var *)(cur->as_ptr))->key, \
	ft_strlen(key) + 1) == 0);
}

t_error	unset_var(char *arg)
{
	t_list		*cur;
	t_list		*prev;
	t_shell		*shell;

	shell = get_shell_struct();
	prev = NULL;
	cur = shell->env_lst;

	while (cur && !is_same_key(arg, cur))
	{
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
	{
		return (NO_ERR);
	}
	free_env_var((t_env_var *) cur->as_ptr);
	if (cur == shell->env_lst)
	{
		shell->env_lst = cur->next;
	}
	else
	{
		prev->next = cur->next;
	}
	free(cur);
	return (NO_ERR);
}

t_error	unset(char *argv[])
{
	size_t	i;
	t_error	error;

	if (!argv[1])
	{
		return (NO_ERR);
	}
	i = 0;
	error = NO_ERR;
	while (!error && argv[i])
	{
		error = unset_var(argv[i]);
		i++;
	}
	return (error);
}
