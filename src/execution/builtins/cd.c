/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 15:18:12 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/29 14:57:56 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	change_directory(char **argv)
{
	if (argv[1] == NULL)
	{
		chdir(getenv("HOME"));
		return (1);
	}
	else
	{
		if (chdir(argv[1]) == -1)
		{
			printf(" %s: no such directory\n", argv[2]);
			return (-1);
		}
		print_pwd();
	}
	return (0);
}
