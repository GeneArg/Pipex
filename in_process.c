/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:56:42 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/10 13:59:46 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process_in(int fds[2], char *infile, char *cmd, char **envp)
{
	int		fd;
	char	*path;
	int		execstat;
	char	**cmd_split;

	close(fds[0]);
	fd = open(infile, O_RDONLY);
	if (fd == -1)
		print_error("Infile error", 0);
	dup2(fds[1], STDOUT_FILENO);
	dup2(fd, STDIN_FILENO);
	path = find_path(envp, cmd);
	if (!path)
		print_error("Command not found", 1);
	cmd_split = ft_split(cmd, ' ');
	execstat = execve(path, cmd_split, envp);
	if (execstat == -1)
	{
		print_error("Execve error command failed / not found", 1);
		free_split(cmd_split);
	}
	free_split(cmd_split);
}

int	parent_process_in(int fds[2])
{
	int	status;

	close(fds[1]);
	wait(&status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (EXIT_FAILURE);
}

int	in_process(int fds[2], char *infile, char *cmd, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		print_error("Fork error", 0);
	else if (pid == 0)
		child_process_in(fds, infile, cmd, envp);
	else
		return (parent_process_in(fds));
	return (EXIT_SUCCESS);
}
