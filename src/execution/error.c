/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 09:20:13 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/15 11:25:08 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_info(t_info *info)
{
	ft_lstclear(&info->env_lst, free_env_var);
	free(info->cur_dir);
	free(info->old_dir);
	free_ar2((void **)info->our_env);
	free(info->cmd_path);

	rl_clear_history();
}
/*
	do i need to free the whole info struct as well?
	but if i use prev_error for exit, won't freeing it affect that?
*/

void	error(int error, t_info *info)
{
	if (error == ERR_MALLOC)
		write(2, "Minishell: Memory allocation error\n", 35);
	else if (error == ERR_OPEN)
		write(2, "Minishell: Open function error\n", 31);
	else if (error == ERR_PIPE)
		write(2, "Minishell: Pipe function error\n", 31);
	else if (error == ERR_DUP)
		write(2, "Minishell: Dup2 function error\n", 31);
	else if (error == ERR_FORK)
		write(2, "Minishell: Fork function error\n", 31);
	else if (error == ERR_CWD)
		write(2, "Minishell: getcwd function error\n", 33);
	else if (error == ERR_EXEC)
		write(2, "Minishell: exec function error\n", 31);
	else if (error == ERR_CLOSE)
		write(2, "Minishell: close function error\n", 32);
	close(info->std_in);
	close(info->std_out);
	free_info(info);
	write(2, "Exiting Minishell\n", 19);
	exit(info->prev_error);
}

int	error_open(t_cmd_data *cmd, int process, t_info *info)
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
