/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   convert_tlist_2d.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/14 11:56:35 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/16 14:08:14 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_length(t_list *node)
{
	int	length;

	length = 0;
	length += ft_strlen(((t_env_var *)(node->as_ptr))->key);
	if (((t_env_var *)(node->as_ptr))->equal)
		length++;
	length += ft_strlen(((t_env_var *)(node->as_ptr))->value);
	return (length);
}

char	**converter(t_list *head)
{
	t_list	*temp;
	char	**array;
	int		length;
	int		i;

	temp = head;
	i = 0;
	array = (char **) malloc((ft_lstsize(temp) + 1) * sizeof(char *));
	// if (!array)
	// 	exit ; // need an error function or smth here
	temp = head;
	while (temp != NULL)
	{
		length = get_length(temp);
		array[i] = (char *) malloc((length + 1) * sizeof(char));
		array[i] = ft_strjoin(ft_strjoin(((t_env_var *) \
		(temp->as_ptr))->key, "="), ((t_env_var *)(temp->as_ptr))->value);
		i++;
		temp = temp->next;
	}
	array[i] = 0;
	return (array);
}
