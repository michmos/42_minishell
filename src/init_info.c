/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_info.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/15 11:28:31 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/15 11:35:23 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "./minishell.h"

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

void	init_info(t_info **ptr, char **env)
{
	t_info	*info;

	info = ft_calloc(1, sizeof(t_info));
	if (!info)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	info->env_lst = create_envlst(env);
	if (!info->env_lst)
		clean_exit(EXIT_FAILURE);
	info->std_in = dup(STDIN_FILENO); //TODO: protect these calls
	info->std_out = dup(STDOUT_FILENO);
	assign_env_values(&info->cur_dir, info->env_lst, "PWD");
	assign_env_values(&info->old_dir, info->env_lst, "OLDPWD");
	info->prev_error = info->error;
	info->error = 0;
	*ptr = info;
	set_info_struct(*ptr);
}
