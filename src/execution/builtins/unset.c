/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/21 11:10:30 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/14 15:26:06 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	unset_envlst_util(t_list *prev, t_list *cur, t_list *last)
{
	prev->next = cur;
	cur = cur->next;
	last = cur;
}

static void	unset_envlst_util_2(t_list *prev, t_list *cur, t_list *last)
{
	cur = cur->next;
	ft_lstdelone(last, free_env_var);
	prev->next->next = cur;
}

t_list	*unset_envlst(t_list *head, char *argv)
{
	t_env_var	*env_var;
	t_list		*cur;
	t_list		*last;
	t_list		*new;
	t_list		*prev;

	prev = NULL;
	last = NULL;
	env_var = get_env_var(argv);
	if (!env_var)
		return (ft_lstclear(&head, free_env_var), NULL);
	new = ft_lstnew(env_var);
	if (!new)
		return (perror("malloc"), ft_lstclear(&head, free_env_var), NULL);
	cur = head;
	if (env_var->equal || env_var->value)
		return (head);
	else
	{
		while (cur && ft_strncmp(key(cur), key(new), ft_strlen(key(cur))))
			unset_envlst_util(prev, cur, last);
		if (!ft_strncmp(key(cur), key(new), ft_strlen(key(cur))))
			unset_envlst_util_2(prev, cur, last);
	}
	return (head);
}

/*
	if !new
		currently i put everything in one line, but it needs different
		function calling instead of the perror
*/