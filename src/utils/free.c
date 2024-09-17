/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/23 15:11:45 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/11 10:29:32 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_redir(void	*redir)
{
	t_redir	*ptr;

	ptr = (t_redir *) redir;
	free(ptr->filename);
	free(ptr);
}

void	free_cmd(void *cmd)
{
	t_cmd	*ptr;

	ptr = (t_cmd *) cmd;
	ft_free_2d_array((void **)(ptr->args));
	ft_lstclear(&ptr->redir_lst, free_redir);
	free(cmd);
}

void	free_token(void *token)
{
	free(((t_token *)token)->lexeme);
	free(token);
}

void	free_env_var(void *var)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)var;
	free(env_var->key);
	free(env_var->value);
	free(env_var);
}
