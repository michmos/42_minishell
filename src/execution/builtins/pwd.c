/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/03 12:24:47 by mmoser        #+#    #+#                 */
/*   Updated: 2024/08/16 11:32:16 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"



void	pwd(void)
{
	t_info	*shell;

	shell = get_info_struct();
	printf("%s\n", shell->cur_dir);
	return ;
}

// void	pwd(void)
// {
// 	t_shell	*shell;

// 	shell = get_shell_struct();
// 	printf("%s\n", shell->cwd);
// 	return ;
// }
