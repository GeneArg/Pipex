/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:27:28 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/03 13:32:43 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv, char **envp)
{
	int	fds[2];

	if (argc != 5)
		print_error("Usage format: ./pipex filein cmd1 cmd2 fileout");
	if (pipe(fds) == -1)
		print_error("Pipe Error");
	
	
}

void	process(int fds[2], char *infile, char *cmd, char **envp)
{
	int fd;
	int pid;
	int execstat;
	char *path;

	pid = fork();
	if (pid == -1)
		print_error("Fork error");
	else if (pid == 0)
	{
		close(fds[0]);
		fd = open(infile, O_RDONLY);
		if (fd == -1)
			print_error("Infile error");
		dup2(fds[1], STDOUT_FILENO);
		dup2(fd, STDIN_FILENO);
		// find path here
	}
}

void	print_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}