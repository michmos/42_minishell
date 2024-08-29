/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 09:31:22 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/29 15:08:34 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sig(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		write(1, "\n", 1);
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	signal_ctrl_d(char *line)
{
	if (line == NULL || *line == '\0')
	{
		printf("exit\n");
	}
}

int	init_signals(void)
{
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
