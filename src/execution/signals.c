/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 09:31:22 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/14 16:09:48 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sig(int signal)
{
	(void)signal;
	// TODO: needs to read from new line
	//  cause only the ctrl-C will come here
}

void	signal_ctrl_d(t_shell *shell, char **line)
{
	(void)shell;
	if (*line == NULL)
	{
		//  i need to connect them, choose either shell or info
		// TODO: instead of shell, send info struct
		//  in here print exit
		//  close info->std_in and info->std_out
		//  free everything and exit with previous_error
	}
}

void	sigint_handle(int signal)
{
	int	g_signal;

	if (signal == SIGINT)
	{
		g_signal = 42;
		close(0);
		write(1, "\n", 1);
	}
}

void	sigquit_handle(int signal)
{
	if (signal == SIGQUIT)
	{
	}
}

void	init_signals(void)
{
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
}
