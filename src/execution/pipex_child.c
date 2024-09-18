/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_child.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/18 14:44:00 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/18 15:00:56 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_external_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = NULL;
	if (init_cmd_path(&path, cmd->args[0], shell->env) != NO_ERR || \
	cmd->args[0] == NULL)
	{
		clean_exit(DEADLY_ERR);
	}
	check_cmd(path, cmd->args[0]);
	execve(path, cmd->args, shell->env);
	perror("execve");
	clean_exit(127);
}

static t_error	close_pipe(t_shell *shell)
{
	if (shell->cur_cmdline.open_pipe_end != -1 && \
	close(shell->cur_cmdline.open_pipe_end) == -1)
	{
		perror("close");
		return (DEADLY_ERR);
	}
	shell->cur_cmdline.open_pipe_end = -1;
	return (NO_ERR);
}

static t_error	setup_redirs(t_cmd *cmd, char *hd_str)
{
	t_error	error;

	error = set_io_redirs(cmd->redir_lst, hd_str);
	if (error != NO_ERR)
		return (error);
	return (NO_ERR);
}

static void	command_execution(t_cmd *cmd, t_shell *shell)
{
	t_error	error;

	if (!cmd->args[0][0])
	{
		ft_printf_fd(STDERR_FILENO, "%s: \
%s: command not found\n", SHELLNAME, cmd->args[0]);
		clean_exit(127);
	}
	if (get_builtin_type(cmd->args[0]) != NO_BUILTIN)
	{
		error = execute_builtin(cmd->args);
		clean_exit(error);
	}
	else
	{
		execute_external_cmd(cmd, shell);
	}
}

void	child_process(t_cmd *cmd)
{
	t_shell	*shell;
	t_error	error;

	shell = get_shell_struct();
	signal(SIGQUIT, handle_sig_child);
	error = NO_ERR;
	if (close_pipe(shell) != NO_ERR)
		clean_exit(DEADLY_ERR);
	error = setup_redirs(cmd, shell->cur_cmdline.hd_str);
	if (error != NO_ERR)
		clean_exit(error);
	if (!cmd->args[0])
	{
		clean_exit(0);
	}
	command_execution(cmd, shell);
}

// void	child_process(t_cmd *cmd)
// {
// 	t_shell	*shell;
// 	t_error	error;
// 	char	*path;

// 	shell = get_shell_struct();
// 	signal(SIGQUIT, handle_sig_child);
// 	error = NO_ERR;
// 	path = NULL;
// 	if (shell->cur_cmdline.open_pipe_end != -1 && \
// 	close(shell->cur_cmdline.open_pipe_end) == -1)
// 	{
// 		perror("close");
// 		clean_exit(DEADLY_ERR);
// 	}
// 	shell->cur_cmdline.open_pipe_end = -1;
// 	error = set_io_redirs(cmd->redir_lst, shell->cur_cmdline.hd_str);
// 	if (error == DEADLY_ERR || error == ERR)
// 		clean_exit(error);
// 	if (!cmd->args[0])
// 	{
// 		clean_exit(0);
// 	}
// 	else if (!cmd->args[0][0])
// 	{
// 		ft_printf_fd(STDERR_FILENO, "%s: \
// set_exit_code(130);%s: command not found\n", SHELLNAME, cmd->args[0]);
// 		clean_exit(127);
// 	}
// 	if (get_builtin_type(cmd->args[0]) != NO_BUILTIN)
// 	{
// 		error = execute_builtin(cmd->args);
// 		clean_exit(error);
// 	}
// 	else if (init_cmd_path(&path, cmd->args[0], shell->env) != NO_ERR || \
// 	cmd->args[0] == NULL)
// 		clean_exit(DEADLY_ERR);
// 	check_cmd(path, cmd->args[0]);
// 	execve(path, cmd->args, shell->env);
// 	perror("execve");
// 	clean_exit(127);
// }