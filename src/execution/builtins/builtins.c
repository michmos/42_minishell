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

int	check_builtins(t_cmd	*cmd)
{
	int			i;

	i = 0;
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0) // doesn't work yet
		i = 1;
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0) // doesn't work yet
		i = 2;
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		i = 3;
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0) // doesn't work yet
		i = 4;
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0) // doesn't work yet
		i = 5;
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0) // doesn't work yet
		i = 6;
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		i = 7;
	return (i);
}

void	export(t_cmd_data *cmd, t_info *info)
{
	int	i;

	i = 1;
	if (cmd->pars_out->args[i])
	{
		while (cmd->pars_out->args[i])
		{
			info->env_lst = \
			add_to_envlst(info->env_lst, &cmd->pars_out->args[i]);
			i++;
		}
	}
	else
	{
		print_ordered_lst();
	}
}

void	env(t_cmd_data *cmd, t_info *info)
{
	int	i;

	i = 1;
	if (cmd->pars_out->args[i])
	{
		while (cmd->pars_out->args[i])
		{
			info->env_lst = \
			add_to_envlst(info->env_lst, &cmd->pars_out->args[i]);
			i++;
		}
	}
	else
		print_envlst(info->env_lst, 6);
}
/*
	export now only works if you first give it and argument, if you don't it won't work

	env gives everything as it should, but need to work on the thing that would replace the node if you give the same key
*/

int	execute_builtin(t_cmd_data *cmd, char *line, t_info *info)
{
	int	stat;

	stat = 0;
	if (cmd->builtin == 1)
		echo(cmd->pars_out->args);
	else if (cmd->builtin == 2)
		cd(cmd->pars_out->args);
	else if (cmd->builtin == 3)
		pwd();
	else if (cmd->builtin == 4)
		export(cmd, info);
	else if (cmd->builtin == 5)
		unset_envlst(info->env_lst, &cmd->pars_out->args[1]);
	else if (cmd->builtin == 6)
		env(cmd, info);
	else if (cmd->builtin == 7)
		execute_exit(cmd, line, info);
	return (stat);
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

int	reset_io(int og_stdin, int og_stdout)
{
	dup2(og_stdin, STDIN_FILENO); // TODO: protect
	dup2(og_stdout, STDOUT_FILENO);
	return (0);
}

// TODO: maybe change input parameters
t_error set_io_files(t_list	*redir_lst)
{
	int	last_input;
	int	last_output;
	int	*fd_array;
	size_t	redir_count;

	// create fd array
	redir_count = ft_lstsize(redir_lst);
	if (redir_count == 0)
		return (0);
	fd_array = (int *)malloc((redir_count * sizeof(int)));
	if (fd_array == NULL)
	{
		perror("malloc");
		return (SYS_ERR);
	}

	// init fd array
	if (open_files(fd_array, redir_lst) != NO_ERR)
	{
		sfree((void **) &fd_array);
		return (SYS_ERR);
	}

	// redirect
	last_input = in_file(redir_lst);
	last_output = out_file(redir_lst);
	if (last_input > -1)
	{
		if (dup2(fd_array[last_input], STDIN_FILENO) == -1)
			; // TODO: protect
	}
	if (last_output > -1)
	{
		if (dup2(fd_array[last_output], STDOUT_FILENO) == -1)
			; // TODO: protect
	}
	if (close_fd_array(fd_array, redir_count) != NO_ERR)
	{
		sfree((void **) &fd_array);
		return (SYS_ERR);
	}
	sfree((void **) &fd_array);
	return (NO_ERR);
}

t_error	exec_one_builtin(t_cmd *cmd, char *line, t_info *info)
{
	int			stat;
	t_cmd_data	*cmd_data;


	cmd_data = get_cmd_data(cmd);
	if (!cmd_data)
	{
		return (SYS_ERR);
	}
	if (set_io_files(cmd_data->pars_out->redir_lst) != NO_ERR)
		; // TODO: protect
	stat = execute_builtin(cmd_data, line, info);
	reset_io(info->std_in, info->std_out); // TODO: protect
	return (stat);
}

/*
	maybe i should call the dup2_copy function that has error handling implemented
	instead of calling dup2
*/
