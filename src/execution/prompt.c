/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prompt.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/12 09:42:59 by pminialg      #+#    #+#                 */
/*   Updated: 2024/06/28 11:57:06 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	show_prompt(void)
{
	char	cwd[4096];

	ft_putstr_fd(getcwd(cwd, sizeof(cwd)), 1);
	ft_putstr_fd("\n", 1);
}
