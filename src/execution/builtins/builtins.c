/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/10 11:18:30 by pminialg      ########   odam.nl         */
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

int	execute_builtin(int i)
{
	int	stat;

	if (i == 1)
		stat = execute_echo();
	else if (i == 2)
		stat = execute_cd();
	else if (i == 3)
		stat = execute_pwd();
	else if (i == 4)
		stat = execute_export();
	else if (i == 5)
		stat = execute_unset();
	else if (i == 6)
		stat = execute_env();
	else if (i == 7)
		stat = execte_exit();
	return (stat);
}

/*
	execute_builtin for sure needs some more work
	just the overall picture is there
*/

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

/*
	check_builtins can be reworked into the function
	i have at home, so it returns a number and then according to that number 
	i can execute the right builtin, but that number will be in a struct 
	and then it will be passed to a fnction called execute_builtin
	and maybe i want to take into account if the builtin should be a 
	child or parent process
*/

/*
	i have a info struct created at home, so maybe i can add the envlst
	to it, it would save space (in terms of variables allowed per function)
	and would be easier to pass
*/