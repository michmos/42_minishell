/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_paths.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pminialg <pminialg@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/26 14:52:41 by pminialg      #+#    #+#                 */
/*   Updated: 2024/06/27 10:31:20 by pminialg      ########   odam.nl         */
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

t_error	init_cmd_path(char **cmd_path, char *command, char **env)
{
	char	*env_path;
	char	*full_path;
	char	**bin_paths;
	int		i;

	// check current path
	if (access(command, X_OK) == 0)
	{
		full_path = ft_strdup(command);
		if (!full_path)
		{
			perror("malloc");
			return (SYS_ERR);
		}
		*cmd_path = full_path;
		return (NO_ERR);
	}

	*cmd_path = NULL;
	env_path = get_env_path(env);
	if (!env_path)
	{
		return (NO_ERR);
	}
	bin_paths = ft_split(env_path + 5, ':');
	if (!bin_paths)
	{
		return (SYS_ERR);
	}

	i = 0;
	while (bin_paths[i])
	{
		full_path = concat_path(bin_paths[i], command);
		if (!full_path)
		{
			ft_free_2d_array((void **) bin_paths);
			return (SYS_ERR);
		}
		if (access(full_path, X_OK) == 0)
		{
			*cmd_path = full_path;
			return (NO_ERR);
		}
		free(full_path);
		i++;
	}
	return (NO_ERR);
}
