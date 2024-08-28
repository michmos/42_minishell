/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/22 14:04:40 by pminialg      ########   odam.nl         */
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
	if (ft_strncmp(cmd, "echo", 5) == 0) // doesn't work yet
		return (ECHO);
	else if (ft_strncmp(cmd, "cd", 3) == 0) // doesn't work yet
		return (CD);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (PWD);
	else if (ft_strncmp(cmd, "export", 7) == 0) // doesn't work yet
		return (EXPORT);
	else if (ft_strncmp(cmd, "unset", 6) == 0) // doesn't work yet
		return (UNSET);
	else if (ft_strncmp(cmd, "env", 4) == 0) // doesn't work yet
		return (ENV);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (EXIT);
	else
		return (NO_BUILTIN);
}

t_error	export(char **args)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (args[0] && args[1])
	{
		if (add_to_envlst(shell->env_lst, args) != NO_ERR)
		{
			return (SYS_ERR);
		}
	}
	else
	{
		print_ordered_lst();
	}
	return (NO_ERR);
}

t_error	env(char **args)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (add_to_envlst(shell->env_lst, args) != NO_ERR)
	{
		return (SYS_ERR);
	}
	print_envlst(shell->env_lst, 6);
	return (NO_ERR);
}
/*
	export now only works if you first give it and argument, if you don't it won't work

	env gives everything as it should, but need to work on the thing that would replace the node if you give the same key
*/

t_error	execute_builtin(char **args)
{
	t_error	error;
	t_shell	*shell;
	t_builtins type;

	error = NO_ERR;
	type = get_builtin_type(args[0]);
	if (type == ECHO)
		echo(args);
	else if (type == CD)
		error = cd(args);
	else if (type == PWD)
		pwd(); // TODO: should throw error in case of too many arguments
	else if (type == EXPORT)
		error = export(args);
	// else if (type == UNSET)
	// 	error = unset_envlst(args);
	else if (type == ENV)
		error = env(args);
	else if (type == EXIT)
		execute_exit(args);

	if (type == CD || type == EXPORT || type == UNSET || type == ENV)
	{
		shell = get_shell_struct();
		ft_free_2d_array((void **) shell->env);
		shell->env = converter(shell->env_lst);
		if (!shell->env)
		{
			return (SYS_ERR);
		}
	}
	return (error);
}
/*
	builtin == 5 (unset)
		- instead of line i need to send cmd->pars_out->args
			and unset everything like bash would with whats given to me at args[0]...
		- micha is currently working on unset
	builtin == 6 (env)
		same goes here, the list should be already created and i just want to add to it
		doesn't make any sense to create it every single time... 
*/

t_error	reset_io(void)
{
	t_shell	*shell;

	shell = get_shell_struct();
	if (dup2(shell->std_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (SYS_ERR);
	}
	if (dup2(shell->std_out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		return (SYS_ERR);
	}
	return (0);
}

t_error	exec_one_builtin(t_cmd *cmd)
{
	int			stat;
	char		*hd_str;

	if (exec_hd(&hd_str, cmd->redir_lst) != NO_ERR)
			return (SYS_ERR);
	if (set_io_redirs(cmd->redir_lst, hd_str) != NO_ERR)
	{
		free(hd_str);
		return (SYS_ERR);
	}
	stat = execute_builtin(cmd->args);
	return (stat);
}

/*
	maybe i should call the dup2_copy function that has error handling implemented
	instead of calling dup2
*/
