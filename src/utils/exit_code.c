/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:25:53 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/27 15:27:59 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	code_storage(bool set, int code)
{
	static int	exit_code;

	if (set)
	{
		exit_code = code;
	}
	return (exit_code);
}

void	set_exit_code(int code)
{
	code_storage(true, code);
}

int	get_exit_code(void)
{
	return (code_storage(false, 0));
}
