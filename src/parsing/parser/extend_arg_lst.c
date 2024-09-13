/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   extend_arg_lst.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: mmoser <mmoser@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/23 15:47:41 by mmoser        #+#    #+#                 */
/*   Updated: 2024/09/05 10:45:53 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_expanded_str(t_list **rem_tokens)
{
	t_token	token;
	char	*str;
	char	*temp;

	temp = NULL;
	str = NULL;
	while (*rem_tokens && is_literal(get_token_tag(*rem_tokens)))
	{
		token = pop_token(rem_tokens);
		if (token.tag == WORD || token.tag == DU_QUO)
		{
			if (expand_all_env_vars(&token.lexeme) != NO_ERR)
			{
				free(token.lexeme);
				free(str);
				return (NULL);
			}
		}
		temp = ft_strjoin(str, token.lexeme);
		free(token.lexeme);
		free(str);
		if (!temp)
		{
			perror("malloc");
			return (NULL);
		}
		str = temp;
	}
	return (str);
}

static char	**get_args(t_list **rem_tokens)
{
	char	**arr;
	char	*str;
	t_tag	tag;

	tag = get_token_tag(*rem_tokens);
	str = get_expanded_str(rem_tokens);
	if (!str)
	{
		return (NULL);
	}

	if (tag == WORD)
	{
		arr = ft_split(str, ' ');
	}
	else
	{
		arr = ft_calloc(2, sizeof(char *));
		arr[0] = str;
	}
	if (!arr)
	{
		perror("malloc");
		free(str);
		return (NULL);
	}
	return (arr);
}

static t_error	add_to_arg_lst(t_list **args_lst, char **args)
{
	t_list	*new;
	size_t	i;

	i = 0;
	while (args[i])
	{
		new = ft_lstnew(args[i]);
		if (!new)
		{
			perror("malloc");
			return (DEADLY_ERR);
		}
		ft_lstadd_back(args_lst, new);
		i++;
	}
	return (NO_ERR);
}


t_error	extend_arg_lst(t_list **args_lst, t_list **rem_tokens)
{
	char	*str;
	char	**arr;
	t_tag	tag;

	arr = get_args(rem_tokens);
	if (!arr)
	{
		return (DEADLY_ERR);
	}
	
	if (add_to_arg_lst(args_lst, arr) != NO_ERR)
	{
		ft_free_2d_array((void **) arr);
		return (DEADLY_ERR);
	}
	return (NO_ERR);
}
