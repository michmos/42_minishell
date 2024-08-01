/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 11:09:54 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/22 11:15:36 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list *get_env_lst(void)
{
	t_shell *shell;

	shell = get_shell_struct();
	if (!shell)
		return (NULL);
	return (shell->env_lst);
}

void	set_env_lst(t_list *new_lst)
{
	t_shell	*shell;
	t_list	*lst;

	if (!new_lst)
		return;

	shell = get_shell_struct();
	if (!shell)
		return;
	lst = shell->env_lst;
	free(lst);
	shell->env_lst = new_lst;
}
