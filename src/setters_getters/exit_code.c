/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:25:53 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/26 10:30:02 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_exit_code(int code)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (shell)
	{
		shell->ex_code = code;
	}
}

int	get_exit_code(void)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (shell)
	{
		return (shell->ex_code);
	}
	return (-1);
}
