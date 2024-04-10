/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eagranat <eagranat@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:27:28 by eagranat          #+#    #+#             */
/*   Updated: 2024/04/10 14:21:29 by eagranat         ###   ########.fr       */
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
