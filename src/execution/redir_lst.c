/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redir_lst.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 13:55:02 by pminialg      #+#    #+#                 */
/*   Updated: 2024/07/11 09:48:21 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int in_file(t_cmd *cmd)
{
	t_cmd *temp;
	int result;
	int i;

	temp = cmd;
	i = 0;
	result = -1;
	while (temp->redir_lst)
	{
		if (((t_redir *)(temp->redir_lst))->type == I_RD || ((t_redir *)(temp->redir_lst))->type == I_RD_HD)
			result = i;
		i++;
		temp->redir_lst = temp->redir_lst->next;
	}
	return (result);
}

int out_file(t_cmd *cmd)
{
	t_cmd *temp;
	int result;
	int i;

	temp = cmd;
	i = 0;
	result = -1;
	while (temp->redir_lst)
	{
		if (((t_redir *)(temp->redir_lst))->type == O_RD || ((t_redir *)(temp->redir_lst))->type == O_RD_APP)
			result = i;
		i++;
		temp->redir_lst = temp->redir_lst->next;
	}
	return (result);
}
// t_list	*return_here_doc_in(t_list *head)
// {
// 	t_list	*temp;
// 	t_list	*here_doc;
// 	t_list	*result;

// 	temp = head;
// 	here_doc = get_cmd(temp)->redir_lst;
// 	while (here_doc)
// 	{
// 		if (get_redir(here_doc)->type == I_RD_HD)
// 			result = here_doc;
// 		here_doc = here_doc->next;
// 	}
// 	return (result);
// }

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

// char	*in_file(t_list *head)
// {
// 	t_list	*temp;
// 	t_list	*infile;
// 	t_list	*result;
// 	char	*file;

// 	temp = head;
// 	infile = get_cmd(temp)->redir_lst;
// 	while (infile)
// 	{
// 		if (get_redir(infile)->type == I_RD)
// 			result = infile;
// 		infile = infile->next;
// 	}
// 	file = ft_strdup((get_redir(get_cmd(result)->redir_lst))->filename);
// 	return (file);
// }

// t_redir	*out_file(t_list *head)
// {
// 	t_list	*temp;
// 	t_list	*outfile;
// 	t_redir	*result;

// 	temp = head;
// 	outfile = get_cmd(temp)->redir_lst;
// 	while (outfile)
// 	{
// 		if (get_redir(outfile)->type == O_RD
// 			|| get_redir(outfile)->type == O_RD_APP)
// 			result = get_redir(outfile);
// 		outfile = outfile->next;
// 	}
// 	return (result);
// }

/*
	i believe the issue here is that i'm returning the last file in
	the redirection list, but what i want to do
	is to open every single file given,
	check it's existents, but use only the last one
*/