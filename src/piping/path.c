/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcasaubo <fcasaubo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 20:16:46 by mikus             #+#    #+#             */
/*   Updated: 2024/06/08 12:43:11 by fcasaubo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path_var(char **envp)
{
	int		i;
	char	**splited;
	char	**to_return;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", ft_strlen("PATH=")))
		i++;
	if (!envp[i])
		return (NULL);
	splited = ft_split(envp[i], '=');
	to_return = ft_split(splited[1], ':');
	return (free_array((void **)splited), to_return);
}

char	*get_path(char	*program, char **path)
{
	char	*candidate;
	char	*appended;
	int		i;

	i = 0;
	appended = ft_strjoin("/", program);
	while (path && path[i])
	{
		candidate = ft_strjoin(path[i], appended);
		if (!access(candidate, F_OK))
			return (free_array((void **)path), free(appended), candidate);
		free(candidate);
		i++;
	}
	return (free_array((void **)path), free(appended), NULL);
}

void	check_local(t_command *current)
{
	struct stat	path_stat;
	char		**temp;
	int			i;

	stat(current->command, &path_stat);
	if (!access(current->command, F_OK) && !S_ISDIR(path_stat.st_mode))
	{
		current->path = ft_strdup(current->command);
		temp = ft_split(current->command, '/');
		free(current->command);
		i = 0;
		while (temp[i + 1])
			i++;
		current->command = ft_strdup(temp[i]);
		free_array((void **)temp);
	}
	else
		current->path = NULL;
}

bool	detect_local(t_command *current)
{
	char	*str;
	int		i;

	str = current->command;
	i = -1;
	while (str[++i])
		if (str[i] == '/')
			return (true);
	return (false);
}

bool	resolve_path(t_command *current, char **path)
{
	current->path = NULL;
	check_local(current);
	if (!current->path && !detect_local(current))
		current->path = get_path(current->command, path);
	if (!current->path)
		return (false);
	return (true);
}
