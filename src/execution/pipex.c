/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 09:50:20 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/13 16:49:59 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static void	child_process(t_cmd *cmd, char *hd_str)
{
	t_shell	*shell;
	t_error	error;
	char	*path;

	shell = get_shell_struct();
	error = NO_ERR;
	// close unused fd
	if (shell->open_fd != -1 && close(shell->open_fd) == -1)
	{
		perror("close");
		clean_exit(DEADLY_ERR);
	}
	shell->open_fd = -1;

	signal(SIGQUIT, SIG_DFL);
	error = set_io_redirs(cmd->redir_lst, hd_str);
	if (error == DEADLY_ERR || error == ERR)
		clean_exit(error);
	if (get_builtin_type(cmd->args[0]) != NO_BUILTIN)
	{
		error = execute_builtin(cmd->args);
		clean_exit(error);
	}
	if (!cmd->args[0] || !cmd->args[0][0])
	{
		if (!cmd->args[0][0])
			ft_printf_fd(STDERR_FILENO, "%s: %s: command not found\n", SHELLNAME, cmd->args[0]);
		clean_exit(127);
	}
	else if (init_cmd_path(&path, cmd->args[0], shell->env) != NO_ERR || \
	cmd->args[0] == NULL)
		clean_exit(DEADLY_ERR);
	check_cmd(path, cmd->args[0]);
	execve(path, cmd->args, shell->env);
	perror("execve");
	clean_exit(127);
}

// waits for all childs and retrieves the exit code of the last child
// or of any other child in case an error occurred in another child
void	wait_for_childs(pid_t last_child, int *status)
{
	int	stat;

	signal(SIGINT, handle_sig_child);
	if (last_child > 0)
	{
		waitpid(last_child, status, 0);
	}
	if (*status == 131)
		ft_printf_fd(STDERR_FILENO, "Quit (core dumped)\n");
	while (wait(&stat) != -1)
	{
		if (WEXITSTATUS(stat) == DEADLY_ERR)
			*status = DEADLY_ERR;
	}
}

t_error	cmd_pipeline(t_list *cmd_lst)
{
	int	i;
	int	status;
	pid_t	pid;
	char	*hd_str;
	int	num_cmd;

	i = 0;
	num_cmd = ft_lstsize(cmd_lst);
	while (i < num_cmd)
	{
		// execute heredocs
		if (exec_hd(&hd_str, get_cmd(cmd_lst)->redir_lst) != NO_ERR)
		{
			clean_exit(EXIT_FAILURE);
		}
		if (set_io_pipes(i, num_cmd) != NO_ERR)
		{
			clean_exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == ERROR)
		{
			perror("fork");
			clean_exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			child_process((t_cmd *)(cmd_lst->as_ptr), hd_str);
			// free head and move to next node, good representation is in lstclear in while loop
		}
		// parent
		if (close_unused_fds(i, num_cmd) != NO_ERR)
		{
			clean_exit(EXIT_FAILURE);
		}
		cmd_lst = cmd_lst->next;
		i++;
		reset_io();
	}
	wait_for_childs(pid, &status);
	if (WEXITSTATUS(status) == DEADLY_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
	set_exit_code(WEXITSTATUS(status));
	return (NO_ERR);
}
