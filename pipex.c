/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:43 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/12 19:16:19 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	parent_procces(int *previous_fd, pid_t pid, int *pipefd, int *index,
		int *status)
{
	waitpid(pid, status, 0);
	close(pipefd[1]);
	if (*index != 2)
		close(*previous_fd);
	*previous_fd = pipefd[0];
	*index += 1;
}

static void	child_procces(char *cmd, char **env, int input_fd, int output_fd)
{
	if (dup2(input_fd, STDIN_FILENO) == -1)
		ft_error_exit("dup2 input_fd");
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		ft_error_exit("dup2 output_fd");
	close(input_fd);
	close(output_fd);
	execute(cmd, env);
	ft_error_exit("execve");
}

static void	init_pipe(pid_t *pid, int *pipefd, int index, int argc)
{
	if (index != argc - 2)
	{
		if (pipe(pipefd) == -1)
			ft_error_exit("pipe");
	}
	*pid = fork();
	if (*pid == -1)
		ft_error_exit("fork");
}

static int	prepare_pipex(int *fds, int argc, char **argv)
{
	if (argc < 5)
		ft_error_exit("Usage: ./pipex <file_in> <cmd> ... <cmd> <file_out>");
	fds[0] = open(argv[1], O_RDONLY);
	if (fds[0] == -1)
		ft_error_exit(argv[1]);
	fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fds[1] == -1)
	{
		close(fds[0]);
		ft_error_exit(argv[argc - 1]);
	}
	fds[2] = 0;
	return (2);
}

int	main(int argc, char **argv, char **env)
{
	int		index;
	int		pipefd[2];
	pid_t	pid;
	int		fds[3];
	int		status;

	status = 0;
	index = prepare_pipex(fds, argc, argv);
	while (index < argc - 1)
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
			parent_procces(&fds[2], pid, pipefd, &index, &status);
	}
	return (exit_status(status, fds, pid));
}
