/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:43 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/24 16:24:19 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	parent_procces(char **argv, char **env, int *fd)
{
	int	fd_out;

	fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out == -1)
		error_exit("open archivo entrada");
	dup2(fd[0], STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd[1]);
	execute(argv[3], env);
}

static void	child_procces(char **argv, char **env, int *fd)
{
	int	fd_in;

	fd_in = open(argv[1], O_RDONLY);
	if (fd_in == -1)
		error_exit("open archivo entrada");
	dup2(fd[1], STDOUT_FILENO);
	dup2(fd_in, STDIN_FILENO);
	close(fd[0]);
	execute(argv[2], env);
}

int	main(int argc, char **argv, char **env)
{
	int		pipefd[2];
	pid_t	pid;

	if (argc != 5)
	{
		printf("Uso:%s <file_in> <cmd1> <cmd2> <file_out>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1)
		error_exit("pipe");
	pid = fork();
	if (pid == -1)
		error_exit("pipe Err");
	else if (pid == 0)
		child_procces(argv, env, pipefd);
	waitpid(pid, NULL, 0);
	parent_procces(argv, env, pipefd);
	return (0);
}
