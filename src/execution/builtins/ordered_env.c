/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ordered_env.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/16 11:11:31 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/21 13:31:01 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*key(t_list *node)
{
	return (((t_env_var *)(node->as_ptr))->key);
}

// t_list	*order_list(t_list *head)
// {
// 	t_list	*cur;

// 	cur = head;
// 	// diff = ft_strncmp(key(head), key(new), ft_strlen(key(head)));
// 	// if (diff >= 0)
// 	// 	ft_lstadd_back(&head, new);
// 	// else
// 	// 	ft_lstadd_front(&head, new);
// }




t_list	*create_ordered_envlst(t_list *env)
{
	t_list		*head;
	t_list		*new;
	t_list		*cur;
	t_list		*last;

	head = NULL;
	while (env)
	{
		new = ft_lstnew(env->as_ptr);
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
		env = env->next;
	}
	return (head);
}
