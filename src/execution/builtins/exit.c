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

void	exit_err(char *str, char *line, t_info *info)
{
	(void)line;
	print_num_arg_err(str);
	close(info->std_in);
	close(info->std_out);
	free_info(info);
	exit(255);
}

void	print_num_arg_err(char *str)
{
	ft_putstr_fd("exit\nMinishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2); // looks like it exits bash after this, might be because i'm on os now
}

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

long long	exit_value(char *str, char *line, t_info *info)
{
	unsigned long long	res;
	int					sign;
	int					i;

	res = 0;
	sign = 1;
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			sign = -1;
	while (str[i])
	{
		if (res > ULLONG_MAX / 10)
		{
			print_num_arg_err(str);
			return (255);
		}
		res = (res * 10) + (str[i] - 48);
		i++;
	}
	if ((sign == 1 && res > LLONG_MAX) || (sign == -1 /*&& res - 1 < LLONG_MIN*/))
		exit_err(str, line, info);
	return (res * sign);
}

int	execute_exit(t_cmd_data *cmd, char *line, t_info *info)
{
	int	i;

	(void)line;
	if (!cmd->pars_out->args[1])
	{
		i = 0;
		ft_putstr_fd("exit\n", 1);
	}
	else if (!str_is_num(cmd->pars_out->args[1]))
	{
		i = 255;
		print_num_arg_err(cmd->pars_out->args[1]);
	}
	else if (cmd->pars_out->args[2])
	{
		info->error = 1;
		return (ft_putstr_fd("exit\nMinishell: exit: too many arguments\n", 2), 0);
	}
	else
	{
		i = exit_value(cmd->pars_out->args[1], line, info);
		ft_putstr_fd("exit\n", 1);
	}
	return (close(info->std_in), close(info->std_out),
		free_info(info), exit(i), 0);
}
