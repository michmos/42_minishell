/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   add_to_list.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/21 10:42:28 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/21 13:31:10 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_list	*add_to_envlst(t_list *head, char *argv)
{
	t_list		*new;
	t_env_var	*env_var;

	env_var = get_env_var(argv);
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
	return (head);
}

t_list	*add_to_ordered_envlst(t_list *head, char *argv)
{
	t_list		*new;
	t_env_var	*env_var;
	t_list		*cur;
	t_list		*last;

	env_var = get_env_var(argv);
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
	cur = head;
	while (cur && ft_strncmp(key(cur), key(new), ft_strlen(key(cur))) < 0)
	{
		last = cur;
		cur = cur->next;
	}
	if (cur == head)
		ft_lstadd_front(&head, new);
	else if (!cur)
		ft_lstadd_back(&head, new);
	else
	{
		last->next = new;
		new->next = cur;
	}
	return (head);
}
