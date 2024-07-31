/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 14:09:47 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void print_envlst(t_list *head)
{
	t_list *tmp;

	tmp = head;
	while (tmp != NULL)
	{
		printf("%s", ((t_env_var *)(tmp->as_ptr))->key);
		if (((t_env_var *)(tmp->as_ptr))->equal)
			printf("=");
		printf("%s", ((t_env_var *)(tmp->as_ptr))->value);
		tmp = tmp->next;
	}
}

int	check_builtins(t_list *head)
{
	t_cmd *cmd;
	char *str;
	char *command;
	int i;

	cmd = get_cmd(head);
	str = cmd->args[0];
	command = ft_str_tolower(str);
	i = 0;
	if (ft_strncmp(command, "echo", 5) == 0)
		i = 1;
	else if (ft_strncmp(command, "cd", 3) == 0)
		i = 2;
	else if (ft_strncmp(command, "pwd", 4) == 0)
		i = 3;
	else if (ft_strncmp(command, "export", 7) == 0)
		i = 4;
	else if (ft_strncmp(command, "unset", 6) == 0)
		i = 5;
	else if (ft_strncmp(command, "env", 4) == 0)
		i = 6;
	else if (ft_strncmp(command, "exit", 5) == 0)
		i = 7;
	free(command);
	return (i);
}

int	execute_builtin(t_cmd_data *cmd, char *line, t_info *info)
{
	int	stat;

	if (cmd->builtin == 1)
		stat = execute_echo();
	else if (cmd->builtin == 2)
		stat = execute_cd();
	else if (cmd->builtin == 3)
		stat = execute_pwd();
	else if (cmd->builtin == 4)
		stat = execute_export();
	else if (cmd->builtin == 5)
		stat = execute_unset();
	else if (cmd->builtin == 6)
		stat = execute_env();
	else if (cmd->builtin == 7)
		stat = execute_exit(cmd, line, info);
	return (stat);
}

int	exec_one_builtin(t_list *head, char *line, t_info *info)
{
	int			stat;
	t_cmd_data	*cmd;

	cmd = get_cmd(head);
	if (open_files(cmd, info) == 1)
		return (1);
	if (cmd->last_input > -1)
	{
		dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO);
	}
	if (cmd->last_output > -1)
	{
		dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO);
	}
	close_fd_array(cmd, info);
	stat = execute_builtin(cmd, line, info);
	return (stat);
}