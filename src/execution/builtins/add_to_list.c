/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   add_to_list.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/21 10:42:28 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/05 15:34:39 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_error add_to_envlst(t_list *head, char **argv)
{
	t_list		*new;
	t_env_var	*env_var;
	t_list		*last;

	// if (*argv == NULL)
	// 	return (head);
	env_var = get_env_var(*argv);
	if (!env_var)
	{
		return (DEADLY_ERR);
	}
	new = ft_lstnew(env_var);
	if (!new)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	ft_lstadd_back(&head, new);
	return (NO_ERR);
}
