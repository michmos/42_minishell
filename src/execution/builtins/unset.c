/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/21 11:10:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/23 17:34:02 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_list	*unset_envlst(t_list *head, char *argv)
{
	t_env_var	*env_var;
	t_list		*cur;
	t_list		*last;
	t_list		*new;
	t_list		*prev;

	env_var = get_env_var(argv);
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
	cur = head;
	if (env_var->equal || env_var->value)
		return (head);
	else
	{
		while (cur && ft_strncmp(key(cur), key(new), ft_strlen(key(cur))))
		{
			prev->next = cur;
			cur = cur->next;
			last = cur;
		}
		if (!ft_strncmp(key(cur), key(new), ft_strlen(key(cur))))
		{
			cur = cur->next;
			ft_lstdelone(last, free_env_var);
			prev->next->next = cur;
		}
	}
	return (head);
}
