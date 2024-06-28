/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/06/26 09:45:04 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_envlst(t_list *head)
{
	t_list	*tmp;

	tmp = head;
	while (tmp != NULL)
	{
		printf("%s", ((t_env_var *)(tmp->as_ptr))->key);
		if (((t_env_var *)(tmp->as_ptr))->equal)
			printf("=");
		printf("%s", ((t_env_var *)(tmp->as_ptr))->value);
		tmp = tmp->next;
	}
}

void	check_builtins(char **argv, char **env)
{
	t_list	*head;
	t_list	*head_ordered;
	int		i;

	i = 1;
	head = create_envlst(env);
	head_ordered = create_ordered_envlst(head);
	while (argv[++i]) // this doesn't make sense
	{
		head = add_to_envlst(head, argv[i]);
		head_ordered = add_to_ordered_envlst(head_ordered, argv[i]);
	}
	if (!ft_strncmp(argv[1], "env", 4))
		print_envlst(head);
	if (!ft_strncmp(argv[1], "export", 7))
		print_envlst(head_ordered);
	i = 1;
	if (!ft_strncmp(argv[1], "unset", 6))
	{
		while (argv[++i])
		{
			head = unset_envlst(head, argv[i]);
			head_ordered = unset_envlst(head_ordered, argv[i]);
		}
	}
	if (!ft_strncmp(argv[1], "pwd", 4))
		print_pwd();
	if (!ft_strncmp(argv[1], "cd", 3))
		change_directory(&argv[1]);
	if (!ft_strncmp(argv[1], "echo", 5))
		echo(&argv[1]);
	if (!ft_strncmp(argv[1], "exit", 5))
		exit_bash(&argv[1]);
	if (!ft_strncmp(argv[1], "print", 6))
		handle_signal(SIGUSR1);
}

/*
	while loop with the coment doesn't make sense,
	cause argv can be another command or pipe,
	so this approach is very wrong
	and i need to rework the idea of check_builtins
	cause i'd like to have a function in which i check
	if the given word maches a builtin and if it does
	i call a function and that's it, no need to add or create
	in this function, we want to keep it short and clean
*/