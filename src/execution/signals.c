/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 09:31:22 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/21 09:48:35 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	handle_sig(int signal)
// {
// 	(void)signal;
// 	rl_on_new_line();
// 	write(1, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 1);
// 	rl_redisplay();
// }
//
// void	signal_ctrl_d(t_info *info, char **line)
// {
// 	if (*line == NULL)
// 	{
// 		printf("exit\n"); // TODO: do i need to print more?
// 		free_info_line(info, *line);
// 		close(info->std_in);
// 		close(info->std_out);
// 		exit(info->prev_error);
// 	}
// }
//
// /*
// 	when there's something written in the line (terminal), is does nothing
// 	but if there's nothing it gives a seg fault
// */
//
// void	sigint_handle(int signal)
// {
// 	int	g_signal;
//
// 	if (signal == SIGINT)
// 	{
// 		g_signal = 42;
// 		close(0);
// 		write(1, "\n", 1);
// 	}
// }
//
// void	sigquit_handle(int signal)
// {
// 	if (signal == SIGQUIT)
// 	{
// 		// printf("signal caught:")
// 	}
// }
//
// int	init_signals(void)
// {
// 	signal(SIGINT, handle_sig);
// 	signal(SIGQUIT, SIG_IGN);
// 	return (0);
// }
