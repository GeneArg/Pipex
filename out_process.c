/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:00:43 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/10 14:09:41 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process_out(int fds[2], char *outfile, char *cmd, char **envp)
{
	int		fd;
	char	*path;
	int		execstat;
	char	**cmd_split;

	close(fds[1]);
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd == -1)
		print_error("Outfile error", 0);
	dup2(fds[0], STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	if (!*cmd)
		print_error("Command not found", 0);
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

int	parent_process_out(int fds[2])
{
	int	status;

	close(fds[0]);
	wait(&status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (EXIT_FAILURE);
}

int	out_process(int fds[2], char *outfile, char *cmd, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		print_error("Fork error", 0);
	else if (pid == 0)
		child_process_out(fds, outfile, cmd, envp);
	else
		return (parent_process_out(fds));
	return (EXIT_SUCCESS);
}
