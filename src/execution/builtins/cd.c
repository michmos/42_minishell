/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 15:18:12 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/31 13:54:23 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	change_directory(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	if (argv[1] == NULL)
	{
		chdir(getenv("HOME"));
		return (1);
	}
	else if (i > 2)
		printf(" cd: Too many arguments\n");
	else
	{
		if (chdir(argv[1]) == -1)
			perror("cd: error");
		print_pwd();
	}
	return (0);
}
