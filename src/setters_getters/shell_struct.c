/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_struct.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 10:12:30 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/22 10:27:49 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_shell	*shell_struct_storage(t_shell *new_shell)
{
	static t_shell	*shell;

	if (new_shell)
	{
		shell = new_shell;
	}
	return (shell);
}

t_shell	*get_shell_struct(void)
{
	return (shell_struct_storage(NULL));
}

void	set_shell_struct(t_shell *new_shell)
{
	shell_struct_storage(new_shell);
}
