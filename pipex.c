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

static void	parent_process(t_pipex *pipex, int *previous_fd, int *index)
{
	waitpid(pipex->pid, &pipex->status, 0);
	close(pipex->pipefd[1]);
	if (*index != 2)
		close(*previous_fd);
	*previous_fd = pipex->pipefd[0];
	(*index)++;
}

static void	child_process(char *cmd, char **env, int input_fd, int output_fd)
{
	if (dup2(input_fd, STDIN_FILENO) == -1)
		ft_perror_exit("dup2 input_fd");
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		ft_perror_exit("dup2 output_fd");
	close(input_fd);
	close(output_fd);
	execute(cmd, env);
	ft_error_exit("execve");
}

static void	init_pipe(t_pipex *pipex, int index, int argc)
{
	if (index != argc - 2)
	{
		if (pipe(pipex->pipefd) == -1)
			ft_error_exit("pipe");
	}
	pipex->pid = fork();
	if (pipex->pid == -1)
		ft_error_exit("fork");
}

static int	prepare_pipex(t_pipex *pipex, int argc, char **argv)
{
	pipex->fds[2] = 0;
	pipex->status = 0;
	if ((ft_strncmp(argv[1], "here_doc", 8) == 0 && argc > 5)
		|| (ft_strncmp(argv[1], "here_doc", 8) != 0 && argc > 4))
	{
		pipex->fds[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
				0777);
		if (pipex->fds[1] == -1)
		{
			perror("");
			ft_putstr_fd(" :", STDERR_FILENO);
			ft_putendl_fd(argv[argc - 1], STDERR_FILENO);
		}
	}
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		return (3);
	pipex->fds[0] = open(argv[1], O_RDONLY);
	if (pipex->fds[0] == -1)
	{
		perror("");
		ft_putstr_fd(" :", STDERR_FILENO);
		ft_putendl_fd(argv[1], STDERR_FILENO);
	}
	return (2);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	pipex;
	int		index;

	if (argc < 5)
		ft_error_exit("Usage: ./pipex infile <cmd> ... <cmd> outfile");
	index = prepare_pipex(&pipex, argc, argv);
	while (index < argc - 1)
	{
		init_pipe(&pipex, index, argc);
		if (pipex.pid == 0)
		{
			if (index == 2)
				child_process(argv[index], env, pipex.fds[0], pipex.pipefd[1]);
			else if (index == 3 && ft_strncmp(argv[1], "here_doc", 8) == 0)
				here_doc(argv, argc, pipex.pipefd);
			else if (index == argc - 2)
				child_process(argv[index], env, pipex.fds[2], pipex.fds[1]);
			else
				child_process(argv[index], env, pipex.fds[2], pipex.pipefd[1]);
			exit(0);
		}
		else
			parent_process(&pipex, &pipex.fds[2], &index);
	}
	return (exit_status(&pipex));
}
