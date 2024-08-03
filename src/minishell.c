/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 14:19:35 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/17 14:21:01 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_info(t_info *info, char **env, char **line, int first)
{
	if (first)
	{
		info->cur_dir = getcwd(NULL, 0);
		info->env_lst = create_envlst(env);
		info->std_in = dup(STDIN_FILENO);
		info->std_out = dup(STDOUT_FILENO);
		*line = NULL;
	}
	info->prev_error = info->error;
	info->error = 0;
	info->num_cmd = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_info	info;
	t_list	*head;
	t_list	*temp;
	char	*line;

	temp = head;
	info.num_cmd = ft_lstsize(temp); // maybe these 2 lines need to go in while loop?
	init_info(&info, &env, &line, 1); // first time send 1, to fill the necessary info
	while (1)
	{
		dup2(info.std_in, STDIN_FILENO);
		dup2(info.std_out, STDOUT_FILENO);
		// deal with signals
		init_info(&info, env, &line, 0);
		// readline and parse it???
		if (line)
			add_history(line);
		finalize_cmd(info);
		pipex(head, &info, line);
		free_info(&info, line, 0);
	}
}

/*
	put env in to the info struct in the env_lst struct
	and make a copy of it in the 2d array? cause it will be easier
	to access it for execve

	we can do that in the finalize_cmd function, maybe then I should
	rename it to finalize_info?
*/