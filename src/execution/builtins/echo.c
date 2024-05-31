/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/29 15:03:19 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/30 10:28:40 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	echo(char **argv)
{
	int	i;

	i = 1;
	if (!ft_strncmp(argv[1], "-n", 3))
	{
		i++;
		while (argv[i])
		{
			printf("%s", argv[i]);
			i++;
		}
	}
	else
	{
		while (argv[i])
		{
			printf("%s", argv[i]);
			i++;
		}
		printf("\n");
	}
	return (0);
}
 
 /*
	right now it's the very basic version
	of echo. later I need to add more things
	in case there would be a redirection to 
	a file, so that I could write to that file
 */