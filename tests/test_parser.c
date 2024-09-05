/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_parser.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/02 11:51:51 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/05 10:47:57 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"
#include <stdlib.h>

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
		printf("\n");

		cmds = cmds->next;
		if (cmds)
			printf("\n\n |\n v\n\n");
		i++;
	}
}

int main(int argc, char *argv[], char **env)
{
	t_shell	*shell;
	t_list	*cmd_lst;
	char	*cmd_line;
	t_error	error;

	// use argc argv to avoid compiling error
	argc = argv[0][0];

	init_shell(&shell, env);
	while (1)
	{
		cmd_line = readline("minishell> ");
		if (!cmd_line)
		{
			break;
		}
		else if (!*cmd_line)
		{
			free(cmd_line);
			continue;
		}
		else if (ft_strncmp(cmd_line, "exit", 5) == 0)
		{
			break;
		}
		error = parsing(&cmd_lst, cmd_line);
		if (error == DEADLY_ERR)
		{
			printf("DEADLY_ERR occurred\n");
			free(cmd_line);
			clean_exit(EXIT_FAILURE);
		}
		else if (error == ERR)
		{
			free(cmd_line);
			continue;
		}
		print_cmds(cmd_lst);
		ft_lstclear(&cmd_lst, free_cmd);
		free(cmd_line);
	}
	clean_exit(shell->ex_code);
}
