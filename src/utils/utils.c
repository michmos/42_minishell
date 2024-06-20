/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:17:46 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/18 14:14:06 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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
