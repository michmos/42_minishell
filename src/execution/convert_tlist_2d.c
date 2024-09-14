/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert_tlist_2d.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 11:56:35 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/06 13:07:08 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
