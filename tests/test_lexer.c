/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:51:51 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/26 15:28:09 by mmoser           ###   ########.fr       */
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
		printf("%-14s | %s\n", enum_strs[((t_token *)(head->as_ptr))->tag], ((t_token *)(head->as_ptr))->lexeme);
		head = head->next;
	}
}

t_list	*test_lexer(char *input)
{
	t_parse_str	cmd_line;
	t_list		*head = NULL;

	cmd_line.buf = input;
	cmd_line.buf_len = ft_strlen(input);
	cmd_line.cursor_pos = 0;
	create_token_lst(&head, &cmd_line);
	return (head);
}

int main(int argc, char *argv[], char **env)
{
	t_shell	*shell;
	t_list	*token_lst;
	char	*cmd_line;

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
		token_lst = test_lexer(cmd_line);
		print_token_lst(token_lst);
		ft_lstclear(&token_lst, free_token);
		free(cmd_line);
	}
	clean_exit(shell->ex_code);
}
