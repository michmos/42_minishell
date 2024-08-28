/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:35:12 by mmoser            #+#    #+#             */
/*   Updated: 2024/08/28 10:02:25 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// TODO: make sure everything is cleaned
static void	cleanup_shell(t_shell *shell)
{
	ft_lstclear(&shell->env_lst, free_env_var);
	free(shell->cwd);
	free(shell->old_wd);

	rl_clear_history();
}

void	clean_exit(int exit_code)
{
	// t_shell	*shell;
	//
	// shell = get_shell_struct();
	// cleanup_shell(shell);
	// if (shell->error == SYS_ERR)
	// {
	// 	; // TODO: print error
	// }
	// exit(exit_code);
}

