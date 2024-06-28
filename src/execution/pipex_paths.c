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
	if (result == NULL)
		err_exit("malloc");
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
	write(STDERR_FILENO, "ERROR: PATH not found\n", 22);
	return (NULL);
}

char	*find_command_path(char *command, char **env)
{
	char	*env_path;
	char	*full_path;
	char	**bin_paths;
	int		i;

	env_path = get_env_path(env);
	if (!env_path)
		return (NULL);
	bin_paths = ft_split(env_path + 5, ':');
	if (!bin_paths)
		return (NULL);
	i = 0;
	while (bin_paths[i])
	{
		full_path = concat_path(bin_paths[i], command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}
