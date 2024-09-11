/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/23 15:56:13 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 15:37:58 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_cmd_line(char *cmd_line)
{
	t_error	error;
	t_list	*cmd_lst;

	cmd_lst = NULL;
	add_history(cmd_line);
	error = parsing(&cmd_lst, cmd_line);
	free(cmd_line);
	if (error == DEADLY_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
	else if (error == ERR)
	{
		ft_lstclear(&cmd_lst, free_cmd);
		return ;
	}
	error = execution(cmd_lst);
	ft_lstclear(&cmd_lst, free_cmd);
	if (error || reset_io() != NO_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
}

static void	dup_in_out_err(t_shell *shell)
{
	shell->std_in = dup(STDIN_FILENO);
	shell->std_out = dup(STDOUT_FILENO); // TODO: protect
	shell->std_err = dup(STDERR_FILENO);
}

int	main(int argc, char *argv[], char **env)
{
	t_shell	*shell;
	char	*cmd_line;

	(void)argc;
	(void)argv;
	init_shell(&shell, env);
	while (1)
	{
		dup_in_out_err(shell);
		init_signals();
		cmd_line = readline("minishell> ");
		if (!cmd_line)
		{
			signal_ctrl_d(cmd_line);
			break ;
		}
		else if (!*cmd_line)
		{
			free(cmd_line);
			continue ;
		}
		process_cmd_line(cmd_line);
	}
	clean_exit(shell->ex_code);
}

// removed t_error from main function, seems like we're not
// using it there