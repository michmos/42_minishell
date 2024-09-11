/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_cmd_path.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 14:52:41 by pminialg      #+#    #+#                 */
/*   Updated: 2024/09/11 10:32:00 by pminialg      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*concat_path(char *dir, char *command)
{
	size_t	len1;
	size_t	len2;
	char	*result;

	len1 = ft_strlen(dir);
	len2 = ft_strlen(command);
	result = malloc(len1 + len2 + 2);
	if (!result)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(result, dir, len1 + 1);
	result[len1] = '/';
	ft_strlcpy(result + len1 + 1, command, len2 + 1);
	return (result);
}

char	*get_env_path(char **env)
{
	size_t	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5) != NULL)
			return (env[i]);
		i++;
	}
	write(2, "ERROR: PATH not found\n", 22);
	return (NULL);
}

t_error	get_bin_paths(char ***bin_paths, char **env)
{
	char	*env_path;

	*bin_paths = NULL;
	env_path = get_env_path(env);
	if (!env_path)
	{
		return (NO_ERR);
	}
	*bin_paths = ft_split(env_path + 5, ':');
	if (!*bin_paths)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	return (NO_ERR);
}

static t_error	get_full_path(char **full_path, char *cmd, char **env)
{
	size_t	i;
	char	**bin_paths;
	t_error	error;

	*full_path = NULL;
	error = NO_ERR;
	if (get_bin_paths(&bin_paths, env) != NO_ERR)
		return (DEADLY_ERR);
	else if (!bin_paths)
		return (NO_ERR);
	i = 0;
	while (bin_paths[i])
	{
		*full_path = concat_path(bin_paths[i], cmd);
		if (!*full_path)
		{
			error = DEADLY_ERR;
			break ;
		}
		if (access(*full_path, X_OK) == 0)
			break ;
		sfree((void **) full_path);
		i++;
	}
	return (ft_free_2d_array((void **) bin_paths), error);
}

t_error	init_cmd_path(char **cmd_path, char *cmd, char **env)
{
	*cmd_path = NULL;
	if (access(cmd, X_OK) != 0)
	{
		if (get_full_path(cmd_path, cmd, env) != NO_ERR)
		{
			return (DEADLY_ERR);
		}
		else if (*cmd_path)
		{
			return (NO_ERR);
		}
	}
	*cmd_path = ft_strdup(cmd);
	if (!*cmd_path)
	{
		perror("malloc");
		return (DEADLY_ERR);
	}
	return (NO_ERR);
}
