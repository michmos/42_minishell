/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/23 15:56:13 by mmoser        #+#    #+#                 */
/*   Updated: 2024/08/15 11:26:25 by pminialg      ########   odam.nl         */
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
	if (error == SYS_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
	else if (error == SYN_ERR)
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

int	main(int argc, char *argv[], char **env)
{
	t_shell	*shell;
	char	*cmd_line;
	t_error	error;

	(void)error;
	(void)argc;
	(void)argv;
	init_shell(&shell, env);
	while (1)
	{
		cmd_line = readline("minishell> "); /*SHELLNAME*/
		if (!cmd_line)
		{
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
