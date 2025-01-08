/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:43 by alejhern          #+#    #+#             */
/*   Updated: 2024/09/29 20:47:53 by alejhern         ###   ########.fr       */
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
}

static void	init_pipe(pid_t *pid, int *pipefd, int index, int argc)
{
	if (index != argc - 2)
	{
		if (pipe(pipefd) == -1)
			ft_error_exit("Resource temporarily unavailable");
	}
	*pid = fork();
	if (*pid == -1)
		ft_error_exit("Cannot allocate memory");
}

static int	prepare_pipex(int *fds, int argc, char **argv)
{
	if (argc < 5)
	{
		ft_printf("Uso:%s <file_in> <cmd> ... <cmd> <file_out>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
		fds[2] = 0;
		return (3);
	}
	fds[0] = open(argv[1], O_RDONLY);
	if (fds[0] == -1)
		ft_perror_exit("");
	fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fds[1] == -1)
	{
		close(fds[0]);
		ft_perror_exit("");
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

	index = prepare_pipex(fds, argc, argv);
	while (index < argc - 1)
	{
		init_pipe(&pid, pipefd, index, argc);
		if (pid == 0)
		{
			if (index == 2)
				child_procces(argv[index], env, fds[0], pipefd[1]);
			else if (index == 3 && ft_strncmp(argv[1], "here_doc", 8) == 0)
				here_doc(argv, argc, pipefd);
			else if (index == argc - 2)
				child_procces(argv[index], env, fds[2], fds[1]);
			else
				child_procces(argv[index], env, fds[2], pipefd[1]);
			exit(0);
		}
		else
			parent_procces(&fds[2], pid, pipefd, &index);
	}
	return (0);
}
