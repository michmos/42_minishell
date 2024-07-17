/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_check.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:02:09 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 11:14:45 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_dir_utils(t_cmd *cmd, t_info *info, struct stat file_stat)
{
	if (!(file_stat.st_mode & S_IXUSR))
	{
		// TODO: error message (permission denied)
		info->error = 126;
		exit(126);
	}
	if (execve(cmd->args[0], cmd->args, info->our_env) == -1)
	{
		// TODO: error message (no such file or directory)
		info->error = 127;
		exit(127);
	}
}

void	check_dir(t_cmd *cmd, t_info *info)
{
	struct stat	file_stat;
	int			stat_result;

	if (!path_separator(cmd))
		return ;
	stat_result = stat(cmd->cmd_path, &file_stat);
	if (stat_result == -1)
	{
		// TODO: error message (no such file or directory)
		exit(127);
	}
	else if (S_ISDIR(file_stat.st_mode))
	{
		// TODO: error message (cmd is a directory)
		exit(126);
	}
	else if (S_ISREG(file_stat.st_mode))
		check_dir_utils(cmd, info, file_stat);
}

void	check_cmd_utils(t_cmd *cmd, t_info *info, struct stat file_stat)
{
	if (cmd->path == NULL)
	{
		if (access(cmd->cmd_path, F_OK) || S_ISDIR(file_stat.st_mode))
		{
			// TODO: error message (command not found)
			exit(127);
		}
		else if (access(cmd->cmd_path, X_OK))
		{
			// TODO: error message (permision denied)
			exit(126);
		}
		else
			// maybe check for something else
	}
}

void	check_cmd(t_cmd *cmd, t_info *info)
{
	struct stat	file_stat;
	int			stat_result;

	stat_result = stat(cmd->cmd_path, &file_stat);
	get_cmd_path(cmd, info);
	check_cmd_utils(cmd, info, file_stat);
	if (execve(cmd->args[0], cmd->args, info->our_env) == -1)
	{
		// TODO: error message(command not found)
		exit(127);
	}
}

int	check_executable(t_cmd *cmd, t_info *info)
{
	struct stat	file_stat;
	int			stat_result;

	stat_result = stat(cmd->cmd_path, &file_stat);
	if (stat_result != -1 && (!(file_stat.st_mode & S_IXUSR)))
	{
		// TODO: error message (permision denied)
		exit(126);
	}
	return (0);
}
