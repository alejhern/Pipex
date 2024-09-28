/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:43 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/29 00:56:34 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	parent_procces(int *previous_fd, pid_t pid, int *pipefd, int *index)
{
	waitpid(pid, NULL, 0);
	close(pipefd[1]);
	if (*index != 2)
		close(*previous_fd);
	*previous_fd = pipefd[0];
	*index += 1;
}

static void	child_procces(char *cmd, char **env, int input_fd, int output_fd)
{
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
	close(input_fd);
	close(output_fd);
	execute(cmd, env);
	error_exit("comand not found!");
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
	{
		close(fds[0]);
		error_exit("Error al abrir el archivo de salida");
	}
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
	while (index < argc -1)
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
			parent_procces(&fds[2], pid, pipefd, &index);
	}
}

int	main(int argc, char **argv, char **env)
{
	if (argc < 5)
	{
		printf("Uso:%s <file_in> <cmd1> ... <cmd2> <file_out>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	pipex(argc, argv, env);
	return (0);
}
