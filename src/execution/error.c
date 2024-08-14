/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 09:20:13 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/14 09:20:46 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_info(t_info *info)
{
    // TODO: free the info struct here
    /*
        int num_cmd;
        int **fd;
        int *pid;
        int error;
        int prev_error;
        char *cur_dir;
        char *old_dir;
        int std_in;
        int std_out;
        t_list *env_lst;
        char **our_env;
        char *cmd_path;
        t_cmd *cmd;
    */
}

void	error(int error, t_info *info)
{
	if (error == ERR_MALLOC)
		write(2, "MiniShell: Memory allocation error\n", 35);
	else if (error == ERR_OPEN)
		write(2, "MiniShell: Open function error\n", 31);
	else if (error == ERR_PIPE)
		write(2, "MiniShell: Pipe function error\n", 31);
	else if (error == ERR_DUP)
		write(2, "MiniShell: Dup2 function error\n", 31);
	else if (error == ERR_FORK)
		write(2, "MiniShell: Fork function error\n", 31);
	else if (error == ERR_CWD)
		write(2, "MiniShell: getcwd function error\n", 33);
	else if (error == ERR_EXEC)
		write(2, "MiniShell: exec function error\n", 31);
	else if (error == ERR_CLOSE)
		write(2, "MiniShell: close function error\n", 32);
	close(info->std_in);
	close(info->std_out);
	free_info(info);
	write(2, "Exiting Minishell\n", 19);
	exit(info->prev_error);
}

int	error_open(t_cmd *cmd, int process, t_info *info)
{
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	close_fd_array(cmd, info);
	close_pipes(info);
	if (process == CHILD)
		exit(EXIT_FAILURE);
	else
	{
		close(info->std_in);
		close(info->std_out);
		return (1);
	}
}
