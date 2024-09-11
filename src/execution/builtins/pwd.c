/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/03 12:24:47 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:45:47 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>

void	pwd(void)
{
	t_shell	*shell;

	shell = get_shell_struct();
	printf("%s\n", shell->cwd);
	return ;
}
