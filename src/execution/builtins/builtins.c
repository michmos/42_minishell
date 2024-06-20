/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/05/16 10:02:02 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_list	*head;
	t_list	*tmp;

	if (!ft_strncmp(argv[1], "env", 4))
	{
		head = create_envlst(env);
	}
	tmp = head;
	while (head != NULL)
	{
		printf("%s", ((t_env_var *)(head->as_ptr))->key);
		if (((t_env_var *)(head->as_ptr))->equal)
			printf("=");
		printf("%s", ((t_env_var *)(head->as_ptr))->value);
		head = head->next;
	}
	return (0);
}
