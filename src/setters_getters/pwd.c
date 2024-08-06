/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:56:17 by mmoser            #+#    #+#             */
/*   Updated: 2024/08/06 15:43:11 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_error	update_shell_struct(t_shell *shell, char *new_path)
{
	free(shell->old_wd);
	shell->old_wd = shell->cwd;
	shell->cwd = ft_strdup(new_path);
	if (!shell->cwd)
	{
		perror("malloc");
		return (SYS_ERR);
	}
	return (NO_ERR);
}

static t_error	update_env_lst(t_list *env_lst, char *cwd_ptr, char *old_wd_ptr)
{
	t_list *pwd_node;
	t_list *oldpwd_node;
	
	pwd_node = ft_lstfind(env_lst, has_key, "PWD");
	if (pwd_node)
	{
		free(((t_env_var *) pwd_node->as_ptr)->value);
		((t_env_var *) pwd_node->as_ptr)->value = ft_strdup(cwd_ptr);
		if (!(((t_env_var *) pwd_node->as_ptr)->value))
		{
			perror("malloc");
			return (SYS_ERR);
		}
	}
	oldpwd_node = ft_lstfind(env_lst, has_key, "OLDPWD");
	if (oldpwd_node)
	{
		free(((t_env_var *) oldpwd_node->as_ptr)->value);
		((t_env_var *) oldpwd_node->as_ptr)->value = ft_strdup(old_wd_ptr);
		if (!(((t_env_var *) oldpwd_node->as_ptr)->value))
		{
			perror("malloc");
			return (SYS_ERR);
		}
	}
	return (NO_ERR);
}


t_error	set_pwd(char *new_path)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (!shell)
		return (INP_ERR);

	if (update_shell_struct(shell, new_path) != NO_ERR)
	{
		return (SYS_ERR);
	}
	if (update_env_lst(shell->env_lst, shell->cwd, shell->old_wd) != NO_ERR)
	{
		return (SYS_ERR);
	}
	return (NO_ERR);
}
