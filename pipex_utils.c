/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:52 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/24 16:05:04 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_array(char **arr)
{
	int	index;

	index = 0;
	while (arr[index])
		free(arr[index++]);
	free(arr);
}

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	**tmp_paths;
	char	*path;
	char	*cmd_path;

	while (ft_strnstr(*env, "PATH", 4) == 0)
		env++;
	paths = ft_split(*env + 5, ':');
	if (!paths)
		error_exit("malloc err");
	path = NULL;
	tmp_paths = paths;
	while (*tmp_paths)
	{
		cmd_path = ft_strjoin(*tmp_paths, "/");
		if (!cmd_path)
			break ;
		path = ft_strjoin(cmd_path, cmd);
		free(cmd_path);
		if (!path || access(path, F_OK) == 0)
			break ;
		free(path);
		tmp_paths++;
	}
	free_array(paths);
	return (path);
}

void	execute(char *argv, char **env)
{
	char	**cmd;
	char	*path;
	int		resp;

	cmd = ft_split(argv, ' ');
	if (!cmd)
		error_exit("malloc err");
	path = find_path(cmd[0], env);
	if (!path)
	{
		free_array(cmd);
		error_exit("path not found");
	}
	resp = execve(path, cmd, env);
	free_array(cmd);
	free(path);
	if (resp == -1)
		error_exit("exec err");
}
