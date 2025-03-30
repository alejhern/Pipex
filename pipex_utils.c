/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 03:54:52 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/12 19:52:12 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc(char **argv, int argc, int *pipefd)
{
	char	*ret;

	if (argc < 6)
		ft_error_exit("Usage: ./pipex here_doc LIMITER <cmd> <cmd> outfile");
	while (1)
	{
		ret = NULL;
		ft_putstr_fd("pipe heredoc> ", STDOUT_FILENO);
		ret = get_next_line(STDIN_FILENO);
		if (!ret)
			break ;
		if (ft_strncmp(ret, argv[2], ft_strlen(argv[2])) == 0)
		{
			free(ret);
			break ;
		}
		if (ft_putstr_fd(ret, pipefd[1]) == -1)
			ft_error_exit("Cannot write to file");
		free(ret);
	}
	close(pipefd[1]);
}

int	exit_status(t_pipex *pipex)
{
	waitpid(pipex->pid, &pipex->status, 0);
	if (pipex->status & 0xFF)
		return (128 + (pipex->status & 0x7F));
	else if ((pipex->status >> 8) & 0xFF)
		return ((pipex->status >> 8) & 0xFF);
	return (0);
}
