/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:43 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/28 04:20:33 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/*
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
*/

static void	parent_procces(int *previous_fd, pid_t pid, int *pipefd, int index)
{
	waitpid(pid, NULL, 0);
	close(pipefd[1]);
	if (index != 2)
		close(*previous_fd);
	*previous_fd = pipefd[0];
}

static void	child_procces(char *cmd, char **env, int input_fd, int output_fd)
{
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
	close(input_fd);
	close(output_fd);
	execute(cmd, env);
}

static void	init_pipe(pid_t *pid, int *pipefd, int index, int argc)
{
	if (index != argc - 2)
	{
		if (pipe(pipefd) == -1)
			error_exit("Error al crear el pipe");
	}
	*pid = fork();
	if (*pid == -1)
		error_exit("Error al hacer fork");
}

static void	open_files(int *fds, int argc, char **argv)
{
	fds[0] = open(argv[1], O_RDONLY);
	if (fds[0] == -1)
		error_exit("Error al abrir el archivo de entrada");
	fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fds[1] == -1)
		error_exit("Error al abrir el archivo de salida");
	fds[2] = 0;
}

void	pipex(int argc, char **argv, char **env)
{
	int		pipefd[2];
	pid_t	pid;
	int		index;
	int		fds[3];

	open_files(fds, argc, argv);
	index = 2;
	while (argv[index])
	{
		init_pipe(&pid, pipefd, index, argc);
		if (pid == 0)
		{
			if (index == 2)
				child_procces(argv[index], env, fds[0], pipefd[1]);
			else if (index == argc - 2)
				child_procces(argv[index], env, fds[2], fds[1]);
			else
				child_procces(argv[index], env, fds[2], pipefd[1]);
			exit(0);
		}
		else
			parent_procces(&fds[2], pid, pipefd, index);
		index++;
	}
	close(fds[0]);
	close(fds[1]);
}

int	main(int argc, char **argv, char **env)
{
	if (argc < 5)
	{
		printf("Uso:%s <file_in> <cmd1> <cmd2> <file_out>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	pipex(argc, argv, env);
	return (0);
}
