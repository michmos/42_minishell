/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/23 15:56:13 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/13 14:55:38 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_cmd_line(char *cmd_line, t_shell *shell)
{
	t_error	error;
	t_list	*cmd_lst;

	cmd_lst = NULL;
	add_history(cmd_line);
	error = parsing(&cmd_lst, &cmd_line);
	shell->cur_cmdlst = cmd_lst;
	free(cmd_line);
	if (error == DEADLY_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
	else if (error == ERR)
	{
		ft_lstclear(&cmd_lst, free_cmd);
		set_exit_code(2);
		return ;
	}
	error = execution(cmd_lst);
	ft_lstclear(&shell->cur_cmdlst, free_cmd);
	if (error == DEADLY_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
}

// TODO: delete valrind suppression file for norm
int	main(int argc, char *argv[], char **env)
{
	t_shell	*shell;
	char	*cmd_line;

	(void)argc;
	(void)argv;
	init_shell(&shell, env);
	while (1)
	{
		init_signals(); // TODO: should this happen in the while loop
		cmd_line = readline("minishell> ");
		// if (isatty(fileno(stdin)))
		// 	cmd_line = readline("minishell> ");
		// else
		// {
		// 	char *line;
		// 	line = get_next_line(fileno(stdin));
		// 	cmd_line = ft_strtrim(line, "\n");
		// 	free(line);
		// }
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
		process_cmd_line(cmd_line, shell);
	}
	clean_exit(shell->ex_code);
}

// removed t_error from main function, seems like we're not
// using it there
