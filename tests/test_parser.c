/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:51:51 by mmoser            #+#    #+#             */
/*   Updated: 2024/06/27 15:09:06 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

void	print_token_lst(t_list *head)
{
	char *enum_strs[] = {
		"WHITESPACES",
		"I_RD",
		"I_RD_HD",
		"O_RD",
		"O_RD_APP",
		"PIPE",
		"SI_QUO",
		"DU_QUO",
		"WORD",
		NULL
	};

	while (head)
	{
		printf("[%s:\"%s\"]", enum_strs[((t_token *)(head->as_ptr))->tag], ((t_token *)(head->as_ptr))->lexeme);
		head = head->next;
		if (head)
			printf("  ->  ");
	}
}

void	print_cmds(t_list *cmds)
{
	size_t	i;
	size_t	j;
	t_cmd	*cmd;

	i = 0;
	while(cmds)
	{
		cmd = (t_cmd *)(cmds->as_ptr);
		printf("cmd #%zu\n", i);

		// print args array
		printf("- args arrary: ");
		j = 0;
		while (cmd->args[j])
		{
			printf("\"%s\" ", cmd->args[j]);
			j++;
		}
		printf("\n");

		// print redirs linked list
		printf("- redirections list: ");
		print_token_lst(cmd->redir_lst);

		cmds = cmds->next;
		if (cmds)
			printf("\n\n |\n v\n\n");
		i++;
	}
}

int main(int argc, char *argv[], char **env)
{
	t_list		*cmds;
	t_list		*env_lst;
	t_error	error;

	if (argc != 2)
	{
		printf("Usage '<ARGS>'");
		return (1);
	}
	env_lst = create_envlst(env);
	cmds = NULL;
	error = parsing(&cmds, argv[1], env_lst);
	if (error)
		return (1);
	print_cmds(cmds);
	return (0);
}
