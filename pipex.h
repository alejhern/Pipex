/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:02:52 by alejhern          #+#    #+#             */
/*   Updated: 2025/01/12 19:16:38 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/wait.h>

typedef struct s_pipex
{
	int		pipefd[2];
	pid_t	pid;
	int		fds[3];
	int		status;
}			t_pipex;

void		here_doc(char **argv, int argc, int *pipefd);
int			exit_status(t_pipex *pipex);

#endif
