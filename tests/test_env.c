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

#include "../src/minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_list	*head;
	t_list	*tmp;

	if (argc < 2 || ft_strncmp(argv[1], "env", 4))
	{
		printf("Usage: ./test_env env");
		return (1);
	}
	set_shell_struct(malloc(sizeof(t_shell)));
	set_env_lst(create_envlst(env));
	tmp = get_env_lst();
	while (tmp != NULL)
	{
		printf("%s", ((t_env_var *)(tmp->as_ptr))->key);
		if (((t_env_var *)(tmp->as_ptr))->equal)
			printf("=");
		printf("%s\n", ((t_env_var *)(head->as_ptr))->value);
		head = head->next;
	}
	printf("\n\n");
	printf("found value %s\n", get_env_val_ptr(ft_strdup("PWD")));
	return (0);
}
