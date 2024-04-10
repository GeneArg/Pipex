/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:27:28 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/10 14:13:30 by eagranat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_error(char *str, int n)
{
	write(STDERR_FILENO, str, ft_strlen(str));
	if (n)
		exit(127);
	exit(EXIT_FAILURE);
}

void	free_split(char **split)
{
	char	**tmp;

	tmp = split;
	while (*split)
	{
		free(*split);
		split++;
	}
	free(tmp);
}

char	**get_paths(char **envp)
{
	char	*sub;
	char	**paths;

	while (*envp && !ft_strnstr(*envp, "PATH=", 5))
		envp++;
	sub = ft_substr(*envp, 5, ft_strlen(*envp) - 5);
	paths = ft_split(sub, ':');
	free(sub);
	return (paths);
}

char	*find_path(char **envp, char *cmd)
{
	char	**paths;
	char	*cmd_path;
	char	*sub;
	char	**cmd_split;
	char	**temp_paths;

	cmd_split = ft_split(cmd, ' ');
	cmd = *cmd_split;
	paths = get_paths(envp);
	temp_paths = paths;
	sub = ft_strjoin("/", cmd);
	while (*temp_paths)
	{
		cmd_path = ft_strjoin(*temp_paths, sub);
		if (!access(cmd_path, F_OK))
			break ;
		free(cmd_path);
		cmd_path = NULL;
		temp_paths++;
	}
	free(sub);
	free_split(cmd_split);
	free_split(paths);
	return (cmd_path);
}

// int	out_process(int fds[2], char *outfile, char *cmd, char **envp)
// {
// 	int fd;
// 	pid_t pid;
// 	int execstat;
// 	char *path;
// 	int status;
// 	char **cmd_split;

// 	pid = fork();
// 	if (pid == -1)
// 		print_error("Fork error", 0);
// 	else if (pid == 0)
// 	{
// 		close(fds[1]);
// 		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
// 		if (fd == -1)
// 			print_error("Outfile error", 0);
// 		dup2(fds[0], STDIN_FILENO);
// 		dup2(fd, STDOUT_FILENO);
// 		if (!*cmd)
// 			print_error("Command not found", 0);
// 		path = find_path(envp, cmd);
// 		if (!path)
// 			print_error("Command not found", 1);
// 		cmd_split = ft_split(cmd, ' ');
// 		execstat = execve(path, cmd_split, envp);
// 		if (execstat == -1)
// 		{
// 			print_error("Execve error command failed / not found", 1);
// 			free_split(cmd_split);
// 		}
// 		free_split(cmd_split);
// 	}
// 	else
// 	{
// 		close(fds[0]);
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			return (WEXITSTATUS(status));
// 		else
// 			return (EXIT_FAILURE);
// 	}
// 	return (EXIT_SUCCESS);
// }

// int	in_process(int fds[2], char *infile, char *cmd, char **envp)
// {
// 	int fd;
// 	int pid;
// 	int execstat;
// 	int status;
// 	char *path;

// 	pid = fork();
// 	if (pid == -1)
// 		print_error("Fork error", 0);
// 	else if (pid == 0)
// 	{
// 		close(fds[0]);
// 		fd = open(infile, O_RDONLY);
// 		if (fd == -1)
// 			print_error("Infile error", 0);
// 		dup2(fds[1], STDOUT_FILENO);
// 		dup2(fd, STDIN_FILENO);
// 		path = find_path(envp, cmd);
// 		if (!path)
// 			print_error("Command not found", 1);
// 		execstat = execve(path, ft_split(cmd, ' '), envp);
// 		if (execstat == -1)
// 			print_error("Execve error command failed / not found", 1);
// 	}
// 	else
// 	{
// 		close(fds[1]);
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			return (WEXITSTATUS(status));
// 		else
// 			return (EXIT_FAILURE);

// 	}
// 	return (EXIT_SUCCESS);
// }

int	main(int argc, char **argv, char **envp)
{
	int	fds[2];
	int	exit_status;

	if (argc != 5)
		print_error("Usage format: ./pipex filein cmd1 cmd2 fileout", 0);
	if (pipe(fds) == -1)
		print_error("Pipe Error", 0);
	exit_status = in_process(fds, argv[1], argv[2], envp);
	wait(NULL);
	exit_status = out_process(fds, argv[4], argv[3], envp);
	close(fds[0]);
	close(fds[1]);
	return (exit_status);
}
