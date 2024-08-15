/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_check.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:02:09 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/14 15:59:25 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_dir_utils(t_cmd_data *cmd, t_info *info, struct stat file_stat)
{
	if (!(file_stat.st_mode & S_IXUSR))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		info->error = 126;
		exit(126);
	}
	info->our_env = converter(info->env_lst);
	if (execve(cmd->pars_out->args[0], cmd->pars_out->args, info->our_env) == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		info->error = 127;
		exit(127);
	}
}

void	check_dir(t_cmd_data *cmd, t_info *info)
{
	struct stat	file_stat;
	int	stat_result;

	//get_cmd_path(cmd, info); // TODO: this function doesn't exist yet, but implement it with pipex_paths functions
	stat_result = stat(cmd->cmd_path, &file_stat);
	if (stat_result == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit(127);
	}
	else if (S_ISDIR(file_stat.st_mode))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	else if (S_ISREG(file_stat.st_mode))
		check_dir_utils(cmd, info, file_stat);
}

void	check_cmd_utils(t_cmd_data *cmd, t_info *info, struct stat file_stat)
{
	(void)info; // add freeing function for info
	if (cmd->path == NULL)
	{
		if (access(cmd->cmd_path, F_OK) || S_ISDIR(file_stat.st_mode))
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(cmd->pars_out->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		else if (access(cmd->cmd_path, X_OK))
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(cmd->pars_out->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
	}
}

void	check_cmd(t_cmd_data *cmd, t_info *info)
{
	struct stat	file_stat;
	int			stat_result;

	stat_result = stat(cmd->cmd_path, &file_stat);
	check_cmd_utils(cmd, info, file_stat);
	info->our_env = converter(info->env_lst);
	if (execve(cmd->pars_out->args[0], cmd->pars_out->args, \
	info->our_env) == -1)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
}

int	check_executable(t_cmd_data *cmd, t_info *info)
{
	struct stat	file_stat;
	int			stat_result;

	(void)info;
	stat_result = stat(cmd->cmd_path, &file_stat);
	if (stat_result != -1 && (!(file_stat.st_mode & S_IXUSR)))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	return (0);
}

/*
	TODO:
		paths function rewrite that
		need to put the env_list from linked list to 2d array somewhere
*/