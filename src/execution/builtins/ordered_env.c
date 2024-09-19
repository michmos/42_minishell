/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ordered_env.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/16 11:11:31 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/18 13:47:41 by mmoser        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*key(t_list *node)
{
	return (((t_env_var *)(node->as_ptr))->key);
}

static void	put_var(t_env_var *env_var)
{
	printf("declare -x %s", env_var->key);
	if (env_var->equal)
	{
		printf("=");
		if (env_var->value)
		{
			printf("\"%s\"", env_var->value);
		}
	}
	printf("\n");
}

static bool	is_bigger(t_env_var *a, t_env_var *b)
{
	return ((a && !b) || ((a && b) && ((a->key && !b->key) || \
	(ft_strncmp(a->key, b->key, ft_strlen(a->key) + 1)) > 0)));
}

static t_env_var	*get_next_bigger(t_env_var *last_var, t_list *env_lst)
{
	t_env_var	*next_bigger;

	while (env_lst && (is_bigger(last_var, (t_env_var *)(env_lst->as_ptr)) \
	|| (t_env_var *)(env_lst->as_ptr) == last_var))
	{
		env_lst = env_lst->next;
	}
	next_bigger = (t_env_var *) env_lst->as_ptr;
	while (env_lst)
	{
		if (is_bigger((t_env_var *)(env_lst->as_ptr), last_var)
		&& is_bigger(next_bigger, (t_env_var *)(env_lst)->as_ptr))
		{
			next_bigger = (t_env_var *)(env_lst->as_ptr);
		}
		env_lst = env_lst->next;
	}
	return (next_bigger);
}

void	print_ordered_lst(void)
{
	t_list		*env_lst;
	size_t		lst_size;
	size_t		i;
	t_env_var	*env_var;

	env_var = NULL;
	env_lst = get_env_lst();
	lst_size = ft_lstsize(env_lst);
	i = 0;
	while (i < lst_size)
	{
		env_var = get_next_bigger(env_var, env_lst);
		put_var(env_var);
		i++;
	}
}
