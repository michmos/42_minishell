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
	t_error		error;

	if (exec_hd( cmd->redir_lst) != NO_ERR)
		return (DEADLY_ERR);
	error = set_io_redirs(cmd->redir_lst, get_shell_struct()->cur_cmdline.hd_str);
	if (error == NO_ERR)
	{
		error = execute_builtin(cmd->args);
	}
	sfree((void **) &get_shell_struct()->cur_cmdline.hd_str);
	reset_io();
	set_exit_code(error);
	return (error);
}
