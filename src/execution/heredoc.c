/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:16:48 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/18 13:11:21 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_error	append_hd_line(char **hd_str, char *tmp)
{
	char	*tmp2;
	char	*new_line;

	tmp2 = ft_strjoin(tmp, "\n");
	if (!tmp2)
		return (perror("malloc"), free(hd_str), DEADLY_ERR);
	free(tmp);
	new_line = ft_strjoin(*hd_str, tmp2);
	free(tmp2);
	free(*hd_str);
	if (!new_line)
		return (perror("malloc"), DEADLY_ERR);
	*hd_str = new_line;
	return (NO_ERR);
}

static t_error	get_hd_str(char **result, char *delimiter)
{
	char	*tmp;
	char	*hd_str;
	t_error	error;

	hd_str = NULL;
	while (true)
	{
		signal(SIGINT, handle_sig_hd);
		tmp = readline("> ");
		if (!tmp)
		{
			ft_printf_fd(STDERR_FILENO, "%s: warning: here-document \
delimited by signal (wanted `%s')\n", SHELLNAME, delimiter);
			break ;
		}
		if (ft_strncmp(tmp, delimiter, ft_strlen(tmp) + 1) == 0)
		{
			free(tmp);
			break ;
		}
		error = append_hd_line(&hd_str, tmp);
		if (error != NO_ERR)
			return (error);
	}
	*result = hd_str;
	return (NO_ERR);
}

t_error	exec_hd(t_list *redir_lst)
{
	char		*result;

	result = NULL;
	while (redir_lst)
	{
		if (get_redir(redir_lst)->type == I_RD_HD)
		{
			free(result);
			if (get_hd_str(&result, get_redir(redir_lst)->filename) != NO_ERR)
			{
				return (DEADLY_ERR);
			}
		}
		redir_lst = redir_lst->next;
	}
	get_shell_struct()->cur_cmdline.hd_str = result;
	return (NO_ERR);
}
