/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 09:31:25 by pminialg      #+#    #+#                 */
/*   Updated: 2024/06/12 09:52:32 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("Current working dir: %s\n", cwd);
	}
	else
	{
		perror("getcwd() error");
		return ;
	}
}

/*
	i might want to update the env lists pwd variable
	with the current working directory,
	cause i don't know if it would do that automatically.
	because we'll be running the program non stop
	in a while loop and the env list might need updating
	or maybe i want to do that after the cd call to somewhere
	cause only then the directory will actually change
	and i should update the pwd variable in the env list
*/