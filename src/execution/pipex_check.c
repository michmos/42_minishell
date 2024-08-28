/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_check.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:02:09 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/21 14:09:39 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
//
// static void	check_dir_utils(t_cmd_data *cmd, struct stat file_stat)
// {
// 	if (!(file_stat.st_mode & S_IXUSR))
// 	{
// 		ft_printf_fd(STDERR_FILENO, "%s: %s: permission denied\n", SHELLNAME, cmd->pars_out->args[0]);
// 		exit(126);
// 	}
// 	if (execve(cmd->path, cmd->pars_out->args, get_shell_struct()->env) == -1)
// 	{
// 		ft_printf_fd(STDERR_FILENO, "%s: %s: no such file or directory\n", SHELLNAME, cmd->pars_out->args[0]);
// 		exit(127);
// 	}
// }


void	check_cmd(char *path, char *arg)
{
	struct stat	file_stat;
	int			stat_result;

	stat_result = stat(path, &file_stat);
	if (stat_result == -1)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: no such file or directory\n", SHELLNAME, arg);
		clean_exit(127);
	}
	else if (S_ISDIR(file_stat.st_mode))
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: is a directory\n", SHELLNAME, arg);
		clean_exit(126);
	}
	else if (S_ISREG(file_stat.st_mode) && !(file_stat.st_mode & S_IXUSR))
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: permission denied\n", SHELLNAME, arg);
		clean_exit(126);
	}
	else if (access(path, F_OK) == -1)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: command not found\n", SHELLNAME, arg);
		clean_exit(127);
	}
	else if (access(path, X_OK) == -1)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: permission denied\n", SHELLNAME, arg);
		clean_exit(126);
	}
}
