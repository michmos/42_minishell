/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoser <mmoser@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 15:25:50 by mmoser            #+#    #+#             */
/*   Updated: 2024/07/30 11:58:08 by mmoser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

void test_cd(char *argv[])
{
	t_shell	*shell;

	shell = get_shell_struct();
	cd(argv);

	printf("struct info:\nold_wd: %s\ncwd: %s\n\n", shell->old_wd, shell->cwd);
}

int main(int argc, char *argv[], char **env)
{
	t_shell	*shell;
	char	*cmd_line;

	// use argc to avoid compiler error
	argc = argv[0][0];

	init_shell(&shell, env);
	printf("struct info:\nold_wd: %s\ncwd: %s\n\n", shell->old_wd, shell->cwd);
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
			clean_exit(shell->ex_code);
		}
		test_cd(ft_split(cmd_line, ' '));
	}
	clean_exit(shell->ex_code);
}
