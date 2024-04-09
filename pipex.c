/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:27:28 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/09 18:59:51 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char **envp, char *cmd)
{
	char	**paths;
	char	*cmd_path;
	char	*sub;

	cmd = *ft_split(cmd, ' ');
	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	sub = ft_substr(*envp, 5, ft_strlen(*envp) - 5);
	paths = ft_split(sub, ':');
	free(sub);
	sub = ft_strjoin("/", cmd);
	while (*paths)
	{
		cmd_path = ft_strjoin(*paths, sub);
		if (!cmd_path)
			return (NULL);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		paths++;
	}
	free(sub);
	free(cmd);
	return (cmd_path);
}

int	out_process(int fds[2], char *outfile, char *cmd, char **envp)
{
	int fd;
	pid_t pid;
	int execstat;
	char *path;
	int status;

	pid = fork();
	if (pid == -1)
		print_error("Fork error");
	else if (pid == 0)
	{
		close(fds[1]);
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		if (fd == -1)
			print_error("Outfile error");
		dup2(fds[0], STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		if (!*cmd)
			print_error("Command not found");
		path = find_path(envp, cmd);
		if (!path)
			print_error("Command not found");
		execstat = execve(path, ft_split(cmd, ' '), envp);
		if (execstat == -1)
		{
			write(STDERR_FILENO, "Execve error command failed / not found\n", 40);
			return (127);
		}
	}
	else
	{
		close(fds[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	in_process(int fds[2], char *infile, char *cmd, char **envp)
{
	int fd;
	int pid;
	int execstat;
	int status;
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
		path = find_path(envp, cmd);
		if (!path)
			print_error("Command not found");
		execstat = execve(path, ft_split(cmd, ' '), envp);
		if (execstat == -1)
		{
			write(STDERR_FILENO, "Execve error command failed / not found\n", 40);
			return (127);
		}
	}
	else
	{
		close(fds[1]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else
			return (EXIT_FAILURE);
	
	}
	return (EXIT_SUCCESS);
}

void	print_error(char *str)
{
	write(STDERR_FILENO, str, ft_strlen(str));
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv, char **envp)
{
	int	fds[2];
	int exit_status;

	if (argc != 5)
		print_error("Usage format: ./pipex filein cmd1 cmd2 fileout");
	if (pipe(fds) == -1)
		print_error("Pipe Error");
	exit_status = in_process(fds, argv[1], argv[2], envp);
	wait(NULL);
	exit_status = out_process(fds, argv[4], argv[3], envp);
	close(fds[0]);
	close(fds[1]);
	return (exit_status);
}
