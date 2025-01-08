/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:52 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/07 19:54:26 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		ft_error_exit("malloc err");
	tmp_paths = paths;
	path = NULL;
	while (*tmp_paths)
	{
		cmd_path = ft_strjoin(*tmp_paths++, "/");
		if (!cmd_path)
			break ;
		path = ft_strjoin(cmd_path, cmd);
		free(cmd_path);
		if (path && access(path, X_OK) == 0)
			break ;
		free(path);
		path = NULL;
	}
	return (ft_free_array((void ***)&paths), path);
}

void	execute(char *argv, char **env)
{
	char	**cmd;
	char	*path;
	int		resp;

	cmd = ft_split(argv, ' ');
	if (!cmd)
		ft_error_exit("Cannot allocate memory");
	if (access(cmd[0], F_OK) == 0)
		path = cmd[0];
	else
		path = find_path(cmd[0], env);
	if (access(path, X_OK) == -1)
	{
		ft_putstr_fd("comand not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		ft_free_array((void ***)&cmd);
		return ;
	}
	resp = execve(path, cmd, env);
	ft_free_array((void ***)&cmd);
	free(path);
	if (resp == -1)
		ft_error_exit("Cannot execute command");
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
			ft_error_exit("Cannot write to file");
		free(ret);
	}
}
