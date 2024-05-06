/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:51:51 by mmoser            #+#    #+#             */
/*   Updated: 2024/05/06 13:41:48 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

void	print_token_lst(t_list *head)
{
	char *enum_strs[] = {
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
		printf("%-10s | %s\n", enum_strs[((t_token *)(head->as_ptr))->tag], ((t_token *)(head->as_ptr))->lexeme);
		head = head->next;
	}
}

int main(int argc, char *argv[])
{
	t_parse_str	cmd_line;
	t_list		*head;

	if (argc != 2)
	{
		printf("Usage '<ARGS>'");
		return (1);
	}
	cmd_line.buf = argv[1];
	cmd_line.buf_len = ft_strlen(argv[1]);
	cmd_line.cursor_pos = 0;
	head = lexer(&cmd_line);
	if (!head)
		return (1);
	print_token_lst(head);
	return (0);
}
