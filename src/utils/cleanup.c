/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/26 14:35:12 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/04 15:17:56 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

static void	cleanup_shell(void)
{
	t_shell *shell;

	shell = get_shell_struct();
	free(shell->cwd);
	free(shell->old_wd);
	if (shell->env_lst)
	{
		ft_lstclear(&(shell->env_lst), free_env_var);
	}
	if (shell->cur_cmdlst)
	{
		ft_lstclear(&(shell->cur_cmdlst), free_cmd);
	}
	ft_free_2d_array((void **) shell->env);
	if (close(shell->std_in) == -1)
	{
		perror("close");
	}
	if (close(shell->std_out) == -1)
	{
		perror("close");
	}
	if (shell->open_fd != -1)
	{
		if (close(shell->open_fd) == -1)
		{
			perror("close");
		}
	}
	free(shell);
}

void	clean_exit(int exit_code)
{
	int	stat;

	wait_for_childs(-1, &stat);
	if (WEXITSTATUS(stat) == SYS_ERR)
	{
		exit_code = SYS_ERR;
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

