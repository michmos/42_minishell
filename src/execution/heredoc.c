/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 11:16:48 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/06 13:58:12 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_error	get_hd_str(char **result, char *delimiter)
{
	char	*tmp;
	char	*tmp2;
	char	*hd_str;

	hd_str = NULL;
	while (true)
	{
		signal(SIGINT, handle_sig_hd);
		tmp = readline("> "); // TODO: protect
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
		tmp2 = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!tmp2)
			return (perror("malloc"), free(hd_str), DEADLY_ERR);
		tmp = ft_strjoin(hd_str, tmp2);
		free(tmp2);
		free(hd_str);
		if (!tmp)
			return (perror("malloc"), DEADLY_ERR);
		hd_str = tmp;
	}
	*result = hd_str;
	return (NO_ERR);
}

t_error	exec_hd(t_list *redir_lst)
{
	char		*result;

	// get heredoc string
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
