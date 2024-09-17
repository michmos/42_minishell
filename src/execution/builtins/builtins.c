/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/13 11:11:06 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdio.h>
#include <unistd.h>

void	print_envlst(t_list *head, int order)
{
	t_list	*tmp;

	tmp = head;
	if (order == 4)
	{
		while (tmp != NULL)
		{
			if (!((t_env_var *)(tmp->as_ptr))->equal)
			{
				printf("declare -x %s\n", ((t_env_var *)(tmp->as_ptr))->key);
			}
			else
			{
				if (((t_env_var *)(tmp->as_ptr))->key)
					printf("declare -x %s", ((t_env_var *)(tmp->as_ptr))->key);
				if (((t_env_var *)(tmp->as_ptr))->equal)
					printf("=");
				if (((t_env_var *)(tmp->as_ptr))->value)
					printf("\"%s\"\n", ((t_env_var *)(tmp->as_ptr))->value);
			}
			tmp = tmp->next;
		}
	}
	else
	{
		while (tmp != NULL)
		{
			if (!((t_env_var *)(tmp->as_ptr))->equal)
			{
				tmp = tmp->next;
			}
			else
			{
				printf("%s", ((t_env_var *)(tmp->as_ptr))->key);
				if (((t_env_var *)(tmp->as_ptr))->equal)
					printf("=");
				printf("%s\n", ((t_env_var *)(tmp->as_ptr))->value);
				tmp = tmp->next;
			}
		}
	}
}

t_builtins	get_builtin_type(char *cmd)
{
	if (cmd == NULL)
		return (NO_BUILTIN);
	else if (ft_strncmp(cmd, "echo", 5) == 0)
		return (ECHO);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (CD);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (PWD);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (EXPORT);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (UNSET);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (ENV);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (EXIT);
	else
		return (NO_BUILTIN);
}

t_error	env(char **args)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (args[1])
		printf("Too many arguments, please type only env\n");
	else
		print_envlst(shell->env_lst, 6);
	set_exit_code(0);
	return (NO_ERR);
}

t_error	execute_builtin(char **args)
{
	t_error		error;
	t_shell		*shell;
	t_builtins	type;

	error = NO_ERR;
	type = get_builtin_type(args[0]);
	if (type == ECHO)
		echo(args);
	else if (type == CD)
		error = cd(args);
	else if (type == PWD)
		pwd();
	else if (type == EXPORT)
		error = exec_export(args);
	else if (type == UNSET)
		error = unset(args);
	else if (type == ENV)
		error = env(args);
	else if (type == EXIT)
		error = exec_exit(args);

	if (type == CD || type == EXPORT || type == UNSET || type == ENV)
	{
		shell = get_shell_struct();
		ft_free_2d_array((void **) shell->env);
		shell->env = converter(shell->env_lst);
		if (!shell->env)
		{
			return (DEADLY_ERR);
		}
	}
	return (error);
}

t_error	exec_one_builtin(t_cmd *cmd)
{
	char		*hd_str;
	t_error		error;

	hd_str = NULL;
	if (exec_hd(&hd_str, cmd->redir_lst) != NO_ERR)
		return (DEADLY_ERR);
	error = set_io_redirs(cmd->redir_lst, hd_str);
	free(hd_str);
	if (error == NO_ERR)
	{
		error = execute_builtin(cmd->args);
	}
	reset_io();
	set_exit_code(error);
	return (error);
}
