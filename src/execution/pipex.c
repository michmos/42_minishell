/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 09:50:20 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/23 14:53:40 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>

static void	child_process(t_cmd *cmd)
{
	t_shell	*shell;
	char	*path;

	shell = get_shell_struct();

	if (get_builtin_type(cmd->args[0]) != NO_BUILTIN)
	{
		if (execute_builtin(cmd->args) != NO_ERR)
		{
			clean_exit(SYS_ERR);
		}
	}
	if (init_cmd_path(&path, cmd->args[0], shell->env) != NO_ERR)
	{
		clean_exit(SYS_ERR);
	}
	check_cmd(path, cmd->args[0]);
	execve(path, cmd->args, shell->env);
	perror("execve");
	clean_exit(127);
}

void	wait_for_childs(pid_t last_child, int *status)
{
	waitpid(last_child, status, 0);  // TODO: protection required?
	while (wait(NULL) != -1)
		;
}

// parent
t_error	close_unused_fds(int pipe[2], int last_rd_end, size_t i, size_t num_childs)
{
	if (i != num_childs - 1)
	{
		if (close(pipe[1]) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}

	}
	if (i != 0)
	{
		if (close(last_rd_end) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
	}
	return (NO_ERR);
}

t_error	set_io_pipes(int pipe[2], int last_rd_end, int child_i, size_t num_childs)
{
	if (child_i > 0)
	{
		if (dup2(last_rd_end, STDIN_FILENO) == -1)
		{
			perror("dup2");
			return (SYS_ERR);
		}
		if (close(last_rd_end) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
	}
	if (child_i < num_childs -1)
	{
		// redirect stdout to pipe
		if (dup2(pipe[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			return (SYS_ERR);
		}
		if (close(pipe[1]) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
		// close unused read end
		if (close(pipe[0]) == -1)
		{
			perror("close");
			return (SYS_ERR);
		}
	}
	return (NO_ERR);
}

t_error	cmd_pipeline(t_list *cmd_lst)
{
	int	i;
	int	status;
	pid_t	pid;
	int		fds[2];
	int		last_rd_end;
	char	*hd_str;
	int	num_cmd;

	i = 0;
	num_cmd = ft_lstsize(cmd_lst);
	while (i < num_cmd)
	{
		if (pipe(fds) == -1)
			wait_free_exit(cmd_lst, EXIT_FAILURE);
		// execute heredocs
		if (exec_hd(&hd_str, get_cmd(cmd_lst)->redir_lst) != NO_ERR)
			return (SYS_ERR);
		pid = fork();
		if (pid == ERROR)
			wait_free_exit(cmd_lst, EXIT_FAILURE);
		else if (pid == 0)
		{
			// set in out
			if (set_io_pipes(fds,	last_rd_end, i, num_cmd) != NO_ERR)
				; // TODO: protect
			if (set_io_redirs(get_cmd(cmd_lst)->redir_lst, hd_str) != NO_ERR)
			{
				exit(SYS_ERR);
				; // TODO: protect
			}

			// signal(SIGQUIT, sigquit_handle);
			child_process((t_cmd *)(cmd_lst->as_ptr)); // TODO: protect
			// free head and move to next node, good representation is in lstclear in while loop
		}
		// parent
		if (close_unused_fds(fds, last_rd_end, i, num_cmd) != NO_ERR)
		{
			; // TODO: protect
		}
		last_rd_end = fds[0];
		cmd_lst = cmd_lst->next;
		i++;
	}
	wait_for_childs(pid, &status);
	set_exit_code(WEXITSTATUS(status));
	return (NO_ERR);
}
