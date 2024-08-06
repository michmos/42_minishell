/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:17:46 by mmoser            #+#    #+#             */
/*   Updated: 2024/08/06 15:58:52 by mmoser           ###   ########.fr       */
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
	{
		perror("malloc");
		return (NULL);
	}

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

bool	has_key(void *var, char *key)
{
	if (!var)
		return (false);
	return (ft_strncmp(key, ((t_env_var *) var)->key, ft_strlen(key) + 1) == 0);
}

char	*get_env_val_ptr(char *key)
{
	t_list	*env_lst;
	t_list	*result_node;

	if (!key)
		return (NULL);

	env_lst = get_env_lst();
	if (!env_lst)
		return (NULL);

	result_node = ft_lstfind(env_lst, has_key, key);
	if (result_node)
		return (((t_env_var *) result_node->as_ptr)->value);
	return (NULL);
}

