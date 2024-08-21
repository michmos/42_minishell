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
	if (execve(cmd->cmd_path, cmd->pars_out->args, info->our_env) == -1)
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

	stat_result = stat(cmd->cmd_path, &file_stat); // I think i was sending wrong thing to stat here, instead of cmd->cmd_path i need to send args[0]
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
	(void)info; // add freeing function for info in case of failur in if statements
	if (cmd->cmd_path == NULL)
	{
		if (access(cmd->pars_out->args[0], F_OK) || S_ISDIR(file_stat.st_mode)) //changing cmd->path to cmd->pars_out->args[0]
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(cmd->pars_out->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		else if (access(cmd->pars_out->args[0], X_OK)) //changing cmd->path to cmd->pars_out->args[0]
		{
			ft_putstr_fd("Minishell: ", 2);
			ft_putstr_fd(cmd->pars_out->args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
		else
			cmd->cmd_path = ft_strdup(cmd->pars_out->args[0]);
	}
}

void	check_cmd(t_cmd_data *cmd, t_info *info)
{
	struct stat	file_stat;
	int			stat_result;

	stat_result = stat(cmd->pars_out->args[0], &file_stat);
	// cmd->path = find_command_path(cmd->pars_out->args[0], info->our_env);
	check_cmd_utils(cmd, info, file_stat);
	if (execve(cmd->cmd_path, cmd->pars_out->args, \
	info->our_env) == -1)
	{
		ft_putstr_fd("Minishell: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
}

/*
	chatgpt said i could be dealing with redirections badly and that's why grep is hanging...
	printf("cmd path = %s\n", cmd->cmd_path);
	printf("cmd args[0] = %s\n", cmd->pars_out->args[0]);
	printf("cmd args[1] = %s\n", cmd->pars_out->args[1]);
*/

int	check_executable(t_cmd_data *cmd, t_info *info)
{
	struct stat	file_stat;
	int			stat_result;

	(void)info; // free info in case of failure as well
	stat_result = stat(cmd->pars_out->args[0], &file_stat);
	if (stat_result != -1 && (!(file_stat.st_mode & S_IXUSR)))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(cmd->pars_out->args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
	return (0);
}
