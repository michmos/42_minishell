/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 09:50:20 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/18 14:44:27 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// waits for all childs and retrieves the exit code of the last child
// or of any other child in case an error occurred in another child
void	wait_for_childs(pid_t last_child, int *status)
{
	int	stat;

	stat = -1;
	signal(SIGINT, handle_sig_child);
	if (last_child > 0)
	{
		waitpid(last_child, status, 0);
	}
	if (*status == 2)
		set_exit_code(130);
	if (*status == 131)
	{
		ft_printf_fd(STDERR_FILENO, "Quit (core dumped)\n");
		set_exit_code(131);
	}
	while (wait(&stat) != -1)
	{
		if (WEXITSTATUS(stat) == DEADLY_ERR)
			*status = DEADLY_ERR;
	}
}

static t_error	prep_and_check_redir(t_list *cmd_lst, int i, int num_cmd)
{
	if (exec_hd(get_cmd(cmd_lst)->redir_lst) != NO_ERR)
	{
		return (DEADLY_ERR);
	}
	if (set_io_pipes(i, num_cmd) != NO_ERR)
	{
		return (DEADLY_ERR);
	}
	return (NO_ERR);
}

static pid_t	handle_execution(t_list *cmd_lst)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
	{
		perror("fork");
		return (ERROR);
	}
	else if (pid == 0)
	{
		child_process((t_cmd *)(cmd_lst->as_ptr));
	}
	if (reset_io() != NO_ERR)
	{
		clean_exit(EXIT_FAILURE);
	}
	sfree((void **) &get_shell_struct()->cur_cmdline.hd_str);
	return (pid);
}

t_error	cmd_pipeline(t_list *cmd_lst)
{
	int		i;
	int		status;
	pid_t	pid;
	int		num_cmd;

	i = 0;
	num_cmd = ft_lstsize(cmd_lst);
	pid = -1;
	set_exit_code(0);
	while (i < num_cmd)
	{
		if (prep_and_check_redir(cmd_lst, i, num_cmd) != NO_ERR)
			clean_exit(EXIT_FAILURE);
		pid = handle_execution(cmd_lst);
		if (pid == ERROR)
			clean_exit(EXIT_FAILURE);
		cmd_lst = cmd_lst->next;
		i++;
	}
	wait_for_childs(pid, &status);
	if (WEXITSTATUS(status) == DEADLY_ERR)
		clean_exit(EXIT_FAILURE);
	if (get_exit_code() != 130 && get_exit_code() != 131)
		set_exit_code(WEXITSTATUS(status));
	return (NO_ERR);
}
