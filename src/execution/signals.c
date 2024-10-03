/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 09:31:22 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/18 09:55:43 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal;

void	handle_sig_hd(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 42;
		if (close(STDIN_FILENO) == -1)
			perror("close");
		if (g_signal == 42)
			write(1, "\n", 1);
		if (close(STDOUT_FILENO) == -1)
			perror("close");
		if (close(STDERR_FILENO) == -1)
			perror("close");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	handle_sig_child(int signal)
{
	if (signal == SIGINT)
	{
		g_signal = 0;
		rl_on_new_line();
		if (g_signal != 42)
			write(1, "\n", 1);
		rl_replace_line("", 1);
	}
}

void	signal_sigquit(int signal)
{
	if (signal == SIGQUIT)
	{
		clean_exit(131);
	}
}

void	handle_sig(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		if (g_signal != 42)
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
	set_exit_code(0);
}

int	init_signals(void)
{
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}
