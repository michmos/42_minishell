/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/30 11:23:41 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/31 10:41:27 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	exit_bash(char **argv)
{
	__uint8_t	num;
	int			i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	if (i > 2)
	{
		errno = E2BIG;
		perror("bash: exit");
		return (1);
	}
	i = 0;
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
		{
			errno = EDOM;
			perror("bash: exit");
			exit(2);
		}
		i++;
	}
	num = ft_atoi(argv[1]);
	printf("exit %d\n", num);
	exit(num);
}

/*
	currently the function checks if it doesn't have too many arguments
	and if the argument provided holds only digits

	need to understand how to access other error codes and how to use them
	but the structure is there
*/

