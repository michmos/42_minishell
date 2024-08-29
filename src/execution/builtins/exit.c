/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/30 11:23:41 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/16 13:20:43 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <unistd.h>


int	str_is_num(char *str)
{
	int	i;

	i = 1;
	if (!ft_isdigit(str[0]) && str[0] != '-' && str[0] != '+')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static t_error 	init_ex_val(unsigned char *result, char *input)
{
	unsigned long long	res;
	int					sign;
	int					i;

	res = 0;
	sign = 1;
	i = 0;
	if (input[i] == '+' || input[i] == '-')
		if (input[i++] == '-')
			sign = -1;
	while (input[i])
	{
		if (res > ULLONG_MAX / 10)
		{
			ft_printf_fd(STDERR_FILENO, "exit\n%s: exit: %s: numeric argument required\n", SHELLNAME, input);
			*result = 2;
			return (OTH_ERR);
		}
		res = (res * 10) + (input[i] - '0');
		i++;
	}
	*result = res * sign;
	return (NO_ERR);
}

void	execute_exit(char **args)
{
	unsigned char	exit_val;
	t_shell	*shell;

	if (!args[1])
	{
		exit_val = 0;
		ft_putstr_fd("exit\n", 1);
	}
	else if (!str_is_num(args[1]))
	{
		exit_val = 255;
		ft_printf_fd(STDERR_FILENO, "exit\n%s: exit: %s: numeric argument required\n", SHELLNAME, args[1]);
	}
	else if (args[2])
	{
		exit_val = 1;
		ft_printf_fd(STDERR_FILENO, "exit\%s: exit: too many arguments\n", SHELLNAME);
	}
	else
	{
		if (init_ex_val(&exit_val, args[1]) == NO_ERR)
		{
			ft_printf("exit\n");
		}
	}
	clean_exit(exit_val);
}
