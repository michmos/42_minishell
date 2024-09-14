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

static void	converter_help_free_clean(int num, char **array)
{
	if (num == 1)
	{
		ft_free_2d_array((void **)array);
	}
	perror("malloc");
	clean_exit(EXIT_FAILURE);
}

char	**converter(t_list *head)
{
	char	*temp;
	char	**array;
	int		i;

	i = 0;
	array = (char **)malloc((ft_lstsize(head) + 1) * sizeof(char *));
	if (!array)
		converter_help_free_clean(0, NULL);
	while (head != NULL)
	{
		temp = ft_strjoin(((t_env_var *)(head->as_ptr))->key, "=");
		if (temp == NULL)
			converter_help_free_clean(1, array);
		array[i] = ft_strjoin(temp, ((t_env_var *)(head->as_ptr))->value);
		i++;
		head = head->next;
	}
	array[i] = 0;
	return (array);
}
