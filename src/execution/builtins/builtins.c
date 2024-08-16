/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 09:25:29 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/16 16:01:53 by pminialg      ########   odam.nl         */
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
		printf("%s\n", ((t_env_var *)(tmp->as_ptr))->value);
		tmp = tmp->next;
	}
}

int	check_builtins(t_list *head, t_cmd	*cmd)
{
	char		*str;
	int			i;

	(void)head;
	str = ft_strdup(cmd->args[0]);
	i = 0;
	if (ft_strncmp(str, "echo", 5) == 0) // doesn't work yet
		i = 1;
	else if (ft_strncmp(str, "cd", 3) == 0) // doesn't work yet
		i = 2;
	else if (ft_strncmp(str, "pwd", 4) == 0)
		i = 3;
	else if (ft_strncmp(str, "export", 7) == 0) // doesn't work yet
		i = 4;
	else if (ft_strncmp(str, "unset", 6) == 0) // doesn't work yet
		i = 5;
	else if (ft_strncmp(str, "env", 4) == 0) // doesn't work yet
		i = 6;
	else if (ft_strncmp(str, "exit", 5) == 0)
		i = 7;
	free(str);
	return (i);
}

/*
	in the function aboce it is impossible for the program to access memory
	in cmd->pars_out->args[0]
	why is this happening? is my malloc bad?
	or something with dereferencing?
*/

int	execute_builtin(t_cmd_data *cmd, char *line, t_info *info)
{
	int	stat;

	stat = 0;
	if (cmd->builtin == 1)
		echo(&line);
	else if (cmd->builtin == 2)
		cd(&line);
	else if (cmd->builtin == 3)
		pwd();
	else if (cmd->builtin == 4)
	{
		create_ordered_envlst(info->env_lst);
		print_envlst(info->env_lst);
	}
	else if (cmd->builtin == 5)
		unset_envlst(info->env_lst, line);
	else if (cmd->builtin == 6)
		print_envlst(info->env_lst);
	else if (cmd->builtin == 7)
		execute_exit(cmd, line, info);
	return (stat);
}
/*
	builtin == 4 (export)
		i want to create the ordered list but at the same time if there anything given i want to add to it
	builtin == 6 (env)
		same goes here, the list should be already created and i just want to add to it
		doesn't make any sense to create it every single time... 
*/
/*
	fix the call names of the function above
	and make sure evry function has an int return value
	that can be used as error if there's any
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
