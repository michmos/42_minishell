/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_envlst.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/09/18 14:02:56 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/18 14:03:38 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	export_print(t_list *head)
{
	while (head != NULL)
	{
		if (!((t_env_var *)(head->as_ptr))->equal)
		{
			printf("declare -x %s\n", ((t_env_var *)(head->as_ptr))->key);
		}
		else
		{
			if (((t_env_var *)(head->as_ptr))->key)
				printf("declare -x %s", ((t_env_var *)(head->as_ptr))->key);
			if (((t_env_var *)(head->as_ptr))->equal)
				printf("=");
			if (((t_env_var *)(head->as_ptr))->value)
				printf("\"%s\"\n", ((t_env_var *)(head->as_ptr))->value);
		}
		head = head->next;
	}
}

static void	env_print(t_list *head)
{
	while (head != NULL)
	{
		if (!((t_env_var *)(head->as_ptr))->equal)
		{
			head = head->next;
		}
		else
		{
			printf("%s", ((t_env_var *)(head->as_ptr))->key);
			if (((t_env_var *)(head->as_ptr))->equal)
				printf("=");
			printf("%s\n", ((t_env_var *)(head->as_ptr))->value);
			head = head->next;
		}
	}
}

void	print_envlst(t_list *head, int order)
{
	if (order == 4)
	{
		export_print(head);
	}
	else
	{
		env_print(head);
	}
}
