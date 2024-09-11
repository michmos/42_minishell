/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   add_to_list.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/21 10:42:28 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/11 10:44:43 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_error	add_to_envlst(t_list *head, char **argv)
{
	t_list		*new;
	t_env_var	*env_var;
	t_list		*cur;
	t_list		*last;
	int			i;
	bool		found;

	found = false;
	i = -1;
	while (argv[++i])
	{
		found = false;
		env_var = get_env_var(argv[i]);
		if (!env_var)
		{
			return (DEADLY_ERR);
		}
		new = ft_lstnew(env_var);
		if (!new)
			return (perror("malloc"), DEADLY_ERR);
		cur = head;
		while (cur)
		{
			if (cur && ft_strncmp(key(new), key(cur), ft_strlen(key(cur)) + 1) == 0)
			{
				found = true;
				cur = cur->next;
				break;
			}
			last = cur;
			cur = cur->next;
		}
		if (found && cur == NULL)
		{
			last->next = new;
			new->next = last->next->next;
			free(cur);
		}
		else if (found)
		{
			last->next = new;
			new->next = cur;
		}
		else
			ft_lstadd_back(&head, new);
	}
	return (NO_ERR);
}
