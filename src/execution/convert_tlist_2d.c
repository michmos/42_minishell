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

static int get_length(t_list *node)
{
	int length;

	length = 0;
	length += ft_strlen(((t_env_var *)(node->as_ptr))->key);
	if (((t_env_var *)(node->as_ptr))->equal)
		length++;
	length += ft_strlen(((t_env_var *)(node->as_ptr))->value);
	return (length);
}

char **converter(t_list *head)
{
	char *temp;
	char **array;
	int length;
	int i;

	i = 0;
	array = (char **)malloc((ft_lstsize(head) + 1) * sizeof(char *));
	// if (!array)
	// 	exit ; // need an error function or smth here
	while (head != NULL)
	{
		length = get_length(head);
		temp = ft_strjoin(((t_env_var *)(head->as_ptr))->key, "=");
		if (temp == NULL)
			return (0); // TODO: maybe want to free array as well?
		array[i] = ft_strjoin(temp, ((t_env_var *)(head->as_ptr))->value);
		i++;
		head = head->next;
	}
	array[i] = 0;
	return (array);
}
