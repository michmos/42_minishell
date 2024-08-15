/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   info_struct.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/08/15 11:27:42 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/15 11:35:37 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_info	*info_struct_storage(t_info *new_info)
{
	static t_info	*info;

	if (new_info)
	{
		info = new_info;
	}
	return (info);
}

t_info	*get_info_struct(void)
{
	return (info_struct_storage(NULL));
}

void	set_info_struct(t_info *new_shell)
{
	info_struct_storage(new_shell);
}
