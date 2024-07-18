/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:16:48 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/18 14:09:57 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void get_hd_str( t_cmd_data *cmd, t_info *info)
{
	char	*input;
	char	*result;
	int		i;

	i = 0;
	result = NULL;
	while (i < cmd->hd_count)
	{
		input = readline("> ");
		/*
			if i have no input i need to see what is the signal, or if there is any
		*/
		if (!input && /*bad signal*/)
			// do something
		else if (input)
		{
			if (ft_strncmp(input, cmd->hd_array[i], ft_strlen(input)) == 0)
				i++;
			else if (i == cmd->hd_count - 1)
				// we want to put only the last string into our cmd->hd_str
				// cause that's the only one that counts
				// while we are on our last heredoc
				// we want to keep adding every sentence to the result
		}
		if (input)
			free(input);
	}
	cmd->hd_str = result;
}
/*
	i created a small test case in a different file
	and there only the last sentence would be saved in input
	need to find a way how to save everything between two heredocs
*/

void init_heredoc(t_cmd_data *cmd, t_info *info)
{
	int i;
	int j;

	i = 0;
	j = 0;
	cmd->hd_array = malloc((cmd->hd_count + 1) * sizeof(char *));
	if (cmd->hd_array == NULL)
		return ;
	while (i < cmd->redir_count)
	{
		/* 
			if cmd->redir->type == I_RD_HD, then we do that
			in hd_array here i want to put the name of the "file" which will
			be the delimiter
		*/
		i++;
	}
	get_hd_str(cmd, info);
}

void	get_hd_fd(t_cmd_data *cmd, t_info *info)
{
	int	fd[2];

	if (pipe(fd) == -1)
		// TODO: Error message
	write(fd[1], cmd->hd_str, ft_strlen(cmd->hd_str));
	close(fd[1]);
	cmd->fd_array[cmd->last_input] = fd[0];
}

void	heredoc(t_cmd_data *cmd, t_info *info, int i)
{
	if (cmd->last_input == i)
		get_hd_fd(cmd, info);
	else
		cmd->fd_array[i] = -2;
}

/*
    basically we are checking if heredoc is the last input
    if it is we'll get the heredoc string and print it to 
    the terminal
    if not we asign -2 and be a check that we don't want to print it
*/