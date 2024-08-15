/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc2.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/19 11:10:16 by pminialg      #+#    #+#                 */
/*   Updated: 2024/08/14 15:57:27 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*hd_strjoin(char *s1, char *s2, t_info *info)
{
	char	*buf;
	int		len;

	(void)info; // remove after error function is implemented
	len = ft_strlen(s1) + ft_strlen(s2) + 2;
	buf = malloc(sizeof(char) * len);
	if (!buf)
		exit(1); // call error function
	if (!s1)
		ft_strlcpy(buf, s2, len);
	else
	{
		ft_strlcpy(buf, s1, len);
		ft_strlcat(buf, s2, len);
	}
	buf[len - 2] = '\n';
	return (buf);
}

void	hd_utils(char **str, char *input, t_info *info, char **result)
{
	*result = hd_strjoin(*str, input, info);
	if (*str)
		free(*str);
	*str = *result;
}

