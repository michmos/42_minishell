/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/26 14:35:12 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 14:58:42 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

static void	cleanup_shell(void)
{
	t_shell	*shell;

	shell = get_shell_struct();
	free(shell->cwd);
	free(shell->old_wd);
	if (shell->env_lst)
	{
		ft_lstclear(&(shell->env_lst), free_env_var);
	}
	free(shell->cur_cmdline.hd_str);
	if (shell->cur_cmdline.cmdlst)
		ft_lstclear(&(shell->cur_cmdline.cmdlst), free_cmd);
	ft_free_2d_array((void **) shell->env);
	if (shell->std_in != -1)
	{
		if (close(shell->std_in) == -1)
			perror("close");

	}
	if (shell->std_out != -1)
	{
		if (close(shell->std_out) == -1)
			perror("close");

	}
	if (shell->std_err != -1)
	{
		if (close(shell->std_err) == -1)
			perror("close");

	}
	if (shell->cur_cmdline.open_pipe_end != -1)
	{
		if (close(shell->cur_cmdline.open_pipe_end) == -1)
			perror("close");
	}
	free(shell);
}

void	clean_exit(int exit_code)
{
	int	stat;

	stat = -1;
	wait_for_childs(-1, &stat);
	if (WEXITSTATUS(stat) == DEADLY_ERR)
	{
		exit_code = DEADLY_ERR;
	}
	cleanup_shell();
	rl_clear_history();
	if (close(STDIN_FILENO) == -1)
	{
		perror("close");
		exit_code = 1;
	}
	if (close(STDOUT_FILENO) == -1)
	{
		perror("close");
		exit_code = 1;
	}
	exit(exit_code);
}
