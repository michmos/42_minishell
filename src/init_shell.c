/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_shell.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/26 14:44:58 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/04 10:46:12 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static void	assign_env_values(char **ptr, t_list *env_lst, const char *env_key)
{
	t_list	*tmp;

	tmp = ft_lstfind(env_lst, has_key, (char *) env_key);
	if (tmp)
	{
		*ptr = ft_strdup(((t_env_var *)tmp->as_ptr)->value);
		if (!*ptr)
		{
			perror("malloc");
			clean_exit(EXIT_FAILURE);
		}
	}
}

void	init_shell(t_shell **ptr, char **env)
{
	t_shell	*shell;

	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	shell->env_lst = create_envlst(env);
	if (!shell->env_lst)
	{
		clean_exit(EXIT_FAILURE);
	}
	shell->env = converter(shell->env_lst);
	if (!shell->env)
	{
		clean_exit(EXIT_FAILURE);
	}
	shell->open_fd = -1;
	shell->ex_code = 0;
	assign_env_values(&shell->cwd, shell->env_lst, "PWD");
	assign_env_values(&shell->old_wd, shell->env_lst, "OLDPWD");
	*ptr = shell;
	set_shell_struct(*ptr);
}
