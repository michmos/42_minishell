/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/21 15:49:49 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_envlst(t_list *head, int order)
{
	t_list	*tmp;

	tmp = head;
	if (order == 4)
	{
		while (tmp != NULL)
		{
			if (((t_env_var *)(tmp->as_ptr))->key)
				printf("declare -x %s", ((t_env_var *)(tmp->as_ptr))->key);
			if (((t_env_var *)(tmp->as_ptr))->equal)
				printf("=");
			if (((t_env_var *)(tmp->as_ptr))->value)
				printf("\"%s\"\n", ((t_env_var *)(tmp->as_ptr))->value);
			tmp = tmp->next;
		}
	}
	else
	{
		while (tmp != NULL)
		{
			printf("%s", ((t_env_var *)(tmp->as_ptr))->key);
			if (((t_env_var *)(tmp->as_ptr))->equal)
				printf("=");
			printf("%s\n", ((t_env_var *)(tmp->as_ptr))->value);
			tmp = tmp->next;
		}
	}
}

int	check_builtins(t_list *head, t_cmd	*cmd)
{
	int			i;

	(void)head;
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

/*
	in the function aboce it is impossible for the program to access memory
	in cmd->pars_out->args[0]
	why is this happening? is my malloc bad?
	or something with dereferencing?
*/

void	export(t_cmd_data *cmd, t_info *info)
{
	t_list	*ordered;

	ordered = NULL;
	info->ordered_lst = create_ordered_envlst(info->env_lst); // maybe we want to already have this in info or shell struct
	if (cmd->pars_out->args[1])
		ordered = add_to_ordered_envlst(info->ordered_lst, &cmd->pars_out->args[1]);
	else
	{
		if (ordered)
			print_envlst(ordered, 4);
		else
			print_envlst(info->ordered_lst, 4);
	}
}
/*
	it works now almost as intended:
	- when only export is typed, it prints everything
	- when more thing are given it doesn't print, but for some reason it doesn't add
		things to the ordered list
	- the ordered list has to be saved, so that every time i type export i could access it

	same thing below with the env
*/

void	env(t_cmd_data *cmd, t_info *info)
{
	if (cmd->pars_out->args[1])
		info->env_lst = add_to_envlst(info->env_lst, &cmd->pars_out->args[1]);
	else
		print_envlst(info->env_lst, 6);
}

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
		print_envlst(info->env_lst, 6);
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

int	exec_one_builtin(t_cmd_data *cmd, char *line, t_info *info)
{
	int			stat;

	if (open_files(cmd, PARENT, info) == 1)
		return (1);
	cmd->fd_array = malloc((sizeof(int) * (2 * info->num_cmd)) + 1);
	if (!cmd->fd_array)
		return (1);
	cmd->last_input = in_file(cmd->pars_out);
	cmd->last_output = out_file(cmd->pars_out);
	if (cmd->last_input > -1)
	{
		dup2_copy(cmd->fd_array[cmd->last_input], STDIN_FILENO, info);
	}
	if (cmd->last_output > -1)
	{
		dup2_copy(cmd->fd_array[cmd->last_output], STDOUT_FILENO, info);
	}
	close_fd_array(cmd, info);
	stat = execute_builtin(cmd, line, info);
	return (stat);
}

/*
	maybe i should call the dup2_copy function that has error handling implemented
	instead of calling dup2
*/
