/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:16:48 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/15 09:37:52 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_msg(char *str)
{
	ft_putstr_fd("Wanted: ", 1);
	ft_putstr_fd(str, 1);
	ft_putstr_fd("\n", 1);
}

static t_error	get_hd_str(char **hd_str, char *delimiter)
{
	char	*tmp;
	char	*tmp2;
	char	*result;

	result = NULL;
	while (true)
	{
		tmp = readline("> "); // TODO: protect
		if (ft_strncmp(tmp, delimiter, ft_strlen(tmp) + 1) == 0)
		{
			break;
		}
		tmp2 = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!tmp2)
		{
			perror("malloc");
			free(result);
			return (SYS_ERR);
		}
		tmp = ft_strjoin(result, tmp2);
		free(tmp2);
		free(result);
		if (!tmp)
		{
			perror("malloc");
			return (SYS_ERR);
		}
		result = tmp;
	}
	*hd_str = result;
	return (NO_ERR);
}

t_error	exec_hd(char **hd_str, t_list *redir_lst)
{
	char	*result;

	// get heredoc string
	result = NULL;
	while (redir_lst)
	{
		if (get_redir(redir_lst)->type == I_RD_HD)
		{
			free(result);
			if (get_hd_str(&result, get_redir(redir_lst)->filename) != NO_ERR)
			{
				return (SYS_ERR);
			}
		}
		redir_lst = redir_lst->next;
	}
	*hd_str = result;
	return (NO_ERR);
}
