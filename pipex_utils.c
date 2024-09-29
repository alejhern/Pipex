/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:52 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/29 19:50:26 by alejhern         ###   ########.fr       */
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

static char	*find_path(char *cmd, char **env)
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

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
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

void	here_doc(char **argv, int argc, int *pipefd)
{
	char	*ret;

	if (argc < 6)
	{
		ft_printf("Uso:%s here_doc DELIMITER <c1> ... <file_out>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	while (1)
	{
		ret = get_next_line(0);
		if (ft_strncmp(ret, argv[2], ft_strlen(argv[2])) == 0)
		{
			free(ret);
			exit(0);
		}
		if (ft_putstr_fd(ret, pipefd[1]) == -1)
			error_exit("write error");
		free(ret);
	}
}
