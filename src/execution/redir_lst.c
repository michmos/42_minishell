/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_lst.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 13:55:02 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/03 16:24:59 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_list	*return_here_doc_in(t_list *head)
{
	t_list	*temp;
	t_list	*here_doc;
	t_list	*result;

	temp = head;
	here_doc = get_cmd(temp)->redir_lst;
	while (here_doc)
	{
		if (get_redir(here_doc)->type == I_RD_HD)
			result = here_doc;
		here_doc = here_doc->next;
	}
	return (result);
}

// t_list	*return_here_doc_out(t_list *head)
// {
// 	t_list	*temp;
// 	t_list	*here_doc;
// 	t_list	*result;

// 	temp = head;
// 	here_doc = get_cmd(temp)->redir_lst;
// 	while (here_doc)
// 	{
// 		if (get_redir(here_doc)->type == O_RD_APP)
// 			result = here_doc;
// 		here_doc = here_doc->next;
// 	}
// 	return (result);
// }

char	*in_file(t_list *head)
{
	t_list	*temp;
	t_list	*infile;
	t_list	*result;
	char	*file;

	temp = head;
	infile = get_cmd(temp)->redir_lst;
	while (infile)
	{
		if (get_redir(infile)->type == I_RD)
			result = infile;
		infile = infile->next;
	}
	file = ft_strdup((get_redir(get_cmd(result)->redir_lst))->filename);
	return (file);
}

t_redir	*out_file(t_list *head)
{
	t_list	*temp;
	t_list	*outfile;
	t_redir	*result;

	temp = head;
	outfile = get_cmd(temp)->redir_lst;
	while (outfile)
	{
		if (get_redir(outfile)->type == O_RD
			|| get_redir(outfile)->type == O_RD_APP)
			result = get_redir(outfile);
		outfile = outfile->next;
	}
	return (result);
}

