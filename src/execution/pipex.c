/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 09:50:20 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/03 16:26:29 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_in_out(size_t child_i, t_list *head, int *pipes, int read_end)
{
	int		fd[2];
	t_redir	*out_redir;

	if (child_i == 0)
		fd[0] = open(in_file(head), O_RDONLY);
	else
		fd[0] = read_end;
	if (!head->next)
	{
		out_redir = out_file(head);
		if (out_redir->type == O_RD_APP)
			fd[1] = open(out_file(head), O_WRONLY | O_APPEND | O_CREAT, 0777);
		else
			fd[1] = open(out_file(head), O_WRONLY | O_TRUNC | O_CREAT, 0777);
	}
	else
	{
		if (close(pipes[0]) == ERROR)
			wait_free_exit(head, EXIT_FAILURE);
		fd[1] = pipes[1];
	}
	if (fd[0] == ERROR || fd[1] == ERROR
		|| i_o_redirection(fd[0], fd[1]) == ERROR)
		wait_free_exit(head, EXIT_FAILURE);
}

/*
	implement changes in the out_file function so that it would return
	the last appending or truncating output redirection as a t_redir node
*/

static int	exec_here_doc(t_list *head, int *pipes)
{
	char	*buffer;
	char	*here_doc;
	size_t	strlen;

	strlen = ft_strlen(out_file(head));
	here_doc = ft_strdup(out_file(head));
	while (1)
	{
		buffer = get_next_line(0);
		if (!buffer)
			wait_free_exit(head, EXIT_FAILURE);
		if (ft_strncmp(buffer, here_doc, strlen) == 0
			&& buffer[strlen] == '\n')
		{
			free(buffer);
			wait_free_exit(head, EXIT_FAILURE);
		}
		write(pipes[1], buffer, ft_strlen(buffer));
		free(buffer);
	}
}

static void	exec_cmd(t_list *head, char **env)
{
	char	*command;

	command = ft_strdup(get_cmd(head)->args[0]);
	execve(find_command_path(command, env), get_cmd(head)->args, env);
	wait_free_exit(head, 127);
}

static void	close_unused_ends(int i, t_list *head, int *pipes, int read_end)
{
	if (head->next)
	{
		if (close(pipes[1]) == ERROR)
			wait_free_exit(head, EXIT_FAILURE);
	}
	if (i != 0)
	{
		if (close(read_end) == ERROR)
			wait_free_exit(head, EXIT_FAILURE);
	}
}

pid_t	cmd_pipeline(t_list *head, int num_childs, char **env)
{
	size_t	i;
	pid_t	pid;
	int		read_end;
	int		pipes[2];

	i = 0;
	read_end = -1;
	while (i < num_childs)
	{
		if (i < num_childs - 1 && pipe(pipes) == ERROR)
			wait_free_exit(head, EXIT_FAILURE);
		pid = fork();
		if (pid == ERROR)
			wait_free_exit(head, EXIT_FAILURE);
		else if (pid == 0)
		{
			if (return_here_doc_in(head))
				exec_here_doc(return_here_doc_in(head), pipes);
			set_in_out(i, head, &pipes, read_end);
			exec_cmd(head, env);
		}
		close_unused_ends(i, head, &pipes, read_end);
		read_end = pipes[0];
		i++;
		head = head->next;
	}
	return (pid);
}
// free the node after use,
// maybe write a function that takes head, frees it and returns head->next

/*
	fix the cmd_pipeline function:
	- it has too many lines;
	- figure out a way to get rid ot change somehow the i and num_childs
		or change the way I add to i;
	- need to free the node I just used;
*/