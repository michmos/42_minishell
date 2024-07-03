/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:17:46 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/03 11:35:02 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**store_ptrs_in_arr(t_list *lst)
{
	size_t	size;
	size_t	i;
	char	**array;

	size = ft_lstsize(lst);
	array = malloc((size + 1) * sizeof(char *));
	if (!array)
		return (NULL);

	i = 0;
	while(i < size)
	{
		array[i] = (char *) lst->as_ptr;
		lst = lst->next;
		i++;
	}
	array[size] = NULL;
	return (array);
}


char	*get_env_val_ptr(char *key, size_t key_len, t_list *env_list)
{
	if (key_len == 0)
	{
		return (NULL);
	}
	while(env_list)
	{
		if (ft_strncmp(key, ((t_env_var *)(env_list->as_ptr))->key, key_len) == 0)
			return (((t_env_var *)(env_list->as_ptr))->value);
		env_list = env_list->next;
	}
	return (NULL);
}

