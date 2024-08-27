/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_env.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/14 10:57:50 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"


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

int	main(int argc, char **argv, char **env)
{
	t_list	*head;
	t_list	*head_ordered;
	t_list	*tmp;
	int		i;

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
	// i = 1;
	// head = create_envlst(env);
	// head_ordered = create_ordered_envlst(head);
	// while (argv[++i])
	// {
	// 	head = add_to_envlst(head, argv[i]);
	// 	head_ordered = add_to_ordered_envlst(head_ordered, argv[i]);
	// }

	return (0);
}

int check_builtins(t_list *head)
{
	t_cmd_data *cmd;
	char *str;
	int i;

	cmd = get_cmd(head);
	str = cmd->pars_out->args[0];
	i = 0;
	if (ft_strncmp(str, "echo", 5) == 0)
		i = 1;
	else if (ft_strncmp(str, "cd", 3) == 0)
		i = 2;
	else if (ft_strncmp(str, "pwd", 4) == 0)
		i = 3;
	else if (ft_strncmp(str, "export", 7) == 0)
		i = 4;
	else if (ft_strncmp(str, "unset", 6) == 0)
		i = 5;
	else if (ft_strncmp(str, "env", 4) == 0)
		i = 6;
	else if (ft_strncmp(str, "exit", 5) == 0)
		i = 7;
	free(str);
	return (i);
}

int execute_builtin(t_cmd_data *cmd, char *line, t_info *info)
{
	int stat;

	if (cmd->builtin == 1)
		echo(&line); // micha
	else if (cmd->builtin == 2)
		cd(&line); // micha
	else if (cmd->builtin == 3)
		pwd(); // micha
	else if (cmd->builtin == 4)
		create_ordered_envlst(info->env_lst); // me -> double check these three cause they've been started a while ago
	else if (cmd->builtin == 5)
		unset_envlst(info->env_lst, line); // me
	else if (cmd->builtin == 6)
		create_envlst(&line); // me
	else if (cmd->builtin == 7)
		execute_exit(cmd, line, info); // me -> done
	return (stat);
}

// int exec_one_builtin(t_list *head, char *line, t_info *info)
// {
// 	int stat;
// 	t_cmd_data *cmd;
//
// 	cmd = get_cmd(head);
// 	if (open_files(cmd, PARENT, info) == 1)
// 		return (1);
// 	if (cmd->last_input > -1)
// 	{
// 		dup2(cmd->fd_array[cmd->last_input], STDIN_FILENO);
// 	}
// 	if (cmd->last_output > -1)
// 	{
// 		dup2(cmd->fd_array[cmd->last_output], STDOUT_FILENO);
// 	}
// 	close_fd_array(cmd, info);
// 	stat = execute_builtin(cmd, line, info);
// 	return (stat);
// }
