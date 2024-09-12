/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_check.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:02:09 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/12 15:56:48 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_err_and_exit(int num, int exit_code, char *arg)
{
	if (num == 1 && exit_code == 127)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: No such file or directory\n", \
		SHELLNAME, arg);
		clean_exit(127);
	}
	else if (num == 2 && exit_code == 126)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: Is a directory\n", SHELLNAME, arg);
		clean_exit(126);
	}
	else if (num == 3 && exit_code == 126)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: Permission denied\n", \
		SHELLNAME, arg);
		clean_exit(126);
	}
	else if (num == 4 && exit_code == 127)
	{
		ft_printf_fd(STDERR_FILENO, "%s: %s: command not found\n", \
		SHELLNAME, arg);
		clean_exit(127);
	}
}

void	check_cmd(char *path, char *arg)
{
	struct stat	file_stat;
	int			stat_result;

	stat_result = stat(path, &file_stat);
	if (stat_result == -1)
		print_err_and_exit(1, 127, arg);
	else if (S_ISDIR(file_stat.st_mode))
		print_err_and_exit(2, 126, arg);
	else if (S_ISREG(file_stat.st_mode) && !(file_stat.st_mode & S_IXUSR))
		print_err_and_exit(3, 126, arg);
	else if (access(path, X_OK) == -1)
		print_err_and_exit(3, 126, arg);
	else if (access(path, F_OK) == -1)
		print_err_and_exit(4, 127, arg);
}
