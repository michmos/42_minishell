/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_helper.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 13:32:12 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/05 10:45:53 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*get_cmd(t_list *lst)
{
	return ((t_cmd *)(lst->as_ptr));
}

t_redir	*get_redir(t_list *lst)
{
	return ((t_redir *)(lst->as_ptr));
}

void	err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

t_error	close_fd(int fd)
{
	if (fd >= 0)
	{
		if (close(fd) == -1)
		{
			perror("close");
			return (DEADLY_ERR);
		}
	}
	return (NO_ERR);
}

char	**converter(t_list *head)
{
	char	*temp;
	char	**array;
	int		i;

	i = 0;
	array = (char **)ft_calloc(ft_lstsize(head) + 1, sizeof(char *));
	if (!array)
	{
		perror("malloc");
		return (NULL);
	}
	while (head != NULL)
	{
		temp = ft_strjoin(((t_env_var *)(head->as_ptr))->key, "=");
		if (!temp)
		{
			perror("malloc");
			ft_free_2d_array((void **)array);

		}
		array[i] = ft_strjoin(temp, ((t_env_var *)(head->as_ptr))->value);
		free(temp);
		if (!array[i])
		{
			perror("malloc");
			ft_free_2d_array((void **)array);

		}
		i++;
		head = head->next;
	}
	array[i] = NULL;
	return (array);
}
