/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 14:19:35 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/16 15:47:37 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_cmd_line(char *cmd_line, t_info *info)
{
	t_error	error;
	t_list	*cmd_lst;

	// cmd_lst = (t_list *)malloc(sizeof(t_list));
	cmd_lst = NULL;
	add_history(cmd_line);
	error = parsing(&cmd_lst, cmd_line);
	if (error == SYS_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
	else if (error == SYN_ERR)
	{
		ft_lstclear(&cmd_lst, free_cmd);
		return ;
	}
	error = execution(cmd_lst, info, cmd_line);
	free(cmd_line);
	ft_lstclear(&cmd_lst, free_cmd_lst);
	if (error)
	{
		clean_exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_info	*info;
	char	*cmd_line;
	t_error	error;

	(void)error;
	(void)argv;
	(void)argc;
	init_info(&info, env);
	while (1)
	{
		cmd_line = readline("minishell> ");
		if (!cmd_line)
			break ;
		else if (!*cmd_line) // i don't understand this line
		{
			free(cmd_line);
			continue ;
		}
		process_cmd_line(cmd_line, info);
	}
	clean_exit(info->error);
}
