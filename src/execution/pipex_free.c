/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_free.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 11:11:50 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/16 10:56:46 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	free_env_var(void *node)
// {
// 	t_env_var	*env_var;

// 	env_var = (t_env_var *) node;
// 	free(env_var->key);
// 	free(env_var->value);
// 	free(env_var);
// }


// static void	free_redir_lst(void *node)
// {
// 	t_redir	*redir_lst;

// 	redir_lst = (t_redir *)node;
// 	free(redir_lst->filename);
// 	free(redir_lst);
// }

/*
	do i need a while loop and go through every node and free them?
	i guess that's logical
	but when would this function be called?
*/

// static void	free_cmd_lst(void *node)
// {
// 	t_cmd	*cmd;

// 	cmd = (t_cmd *) node;
// 	free_ar2((void **) cmd->args);
// 	ft_lstclear(&cmd->redir_lst, free_redir_lst);
// 	free(cmd);
// }

void	wait_free_exit(t_list *head, int exit_status)
{
	ft_lstclear(&head, free_cmd);
	exit (exit_status);
}

/*
	not particularly happy with the free functions
	cause with the dereferencing i don't
	fully grasp the concept if i'm right or not

	but at least the idea is there
*/
