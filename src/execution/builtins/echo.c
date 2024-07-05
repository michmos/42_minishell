/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 12:04:59 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/05 14:46:07 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_n_flag(char *str)
{
	size_t	i;

	if (str[0] != '-')
	{
		return (false);
	}
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
		{
			return (false);
		}
		i++;
	}
	return (true);
}

void	echo(char *argv[])
{
	size_t	i;
	bool	n_option;

	n_option = false;
	i = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		n_option = true;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		i++;
		if (argv[i])
		{
			printf(" ");
		}
	}
	if (!n_option)
	{
		printf("\n");
	}
}
