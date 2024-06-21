/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:17:46 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/21 13:45:38 by mmoser           ###   ########.fr       */
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

char	*get_env_val_ptr(char *key, t_list *env_list)
{
	size_t	key_len;

	key_len = ft_wordlen(key);
	while(env_list)
	{
		if (ft_strncmp(key, ((t_env_var *)(env_list->as_ptr))->key, key_len) == 0)
			return (((t_env_var *)(env_list->as_ptr))->value);
		env_list = env_list->next;
	}
	return (NULL);
}

