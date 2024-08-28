/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:35:12 by mmoser            #+#    #+#             */
/*   Updated: 2024/08/28 16:13:23 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <unistd.h>

void	clean_exit(int exit_code)
{
	cleanup_shell();
	rl_clear_history();
	if (close(STDIN_FILENO) == -1)
	{
		perror("close");
		exit_code = 1;
	}
	if (close(STDOUT_FILENO) == -1)
	{
		perror("close");
		exit_code = 1;
	}
	exit(exit_code);
}

