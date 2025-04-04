/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfarkas <tfarkas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:31:09 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/01 18:47:26 by tfarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_filename(char *cmd)
{
	int		i = 0, j = 0, start;
	char	*filename;

	filename = NULL;
	while (cmd[i] && (cmd[i] == '>' || cmd[i] == '<'))
		i++;
	while (cmd[i] && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n'))
		i++;
	if (!cmd[i])
		return (NULL);
	start = i;
	while (cmd[i] && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
		i++;
	filename = malloc(i - start + 1);
	if (!filename)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (start < i)
		filename[j++] = cmd[start++];
	filename[j] = '\0';
	// printf("filename: %s\n", filename);
	return filename;
}

void	handle_heredoc(const char *delimiter) // ВЫОДИТЬ НИЧЕГО НЕ НАДО!!!
{
	int pipe_fd[2];
	char *line = NULL;

	if (pipe(pipe_fd) == -1) // Создаём пайп
	{
		perror("pipe");
		return ;
	}
	// printf("pipefd[0] == %d; pipefd[1] == %d\n", pipe_fd[0], pipe_fd[1]);
	// printf("delimiter: %s; len: %d\n", delimiter, ft_strlen(delimiter));
	while (1)
	{
		// write(STDOUT_FILENO, "> ", 2);
		line = readline("heredoc> "); // НУЖНО ЗАПОМИНАТЬ LINE, ЧТОБЫ ЕГО ПОТОМ ВЫВЕСТИ
		if (!line || (ft_strncmp(line, delimiter, ft_strlen((char *)delimiter)) == 0 && ft_strlen(line) == ft_strlen((char *)delimiter)))
		{
			free(line);
			break ;
		}
		// printf("line: %s; len: %d\n", line, ft_strlen(line));
			// добавить еще условие на && line[strlen(delimiter)] == '\n'
		write(pipe_fd[1], line, strlen(line)); // Пишем в пайп
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO); // Перенаправляем stdin на пайп
	close(pipe_fd[0]);
}

void	handle_redirection(char *line, int *status)
{
	int		i = 0;
	int		file_fd;
	char	*filename;
	struct s_saved_std std;

	// printf("[handle_redirection] starting....\n");
	// std.saved_stdin = dup(STDIN_FILENO);
	// std.saved_stdout = dup(STDOUT_FILENO);
	while (line[i])
	{
		// printf("[handle_redirection] in cycle...\n");
		if (line[i] == '<' || line[i] == '>')
		{
			filename = get_filename(line + i);
			printf("filename: %s\n", filename);
			if (!filename)
			{
				printf("no name for file\n");
				*status = 1;
				return ;
			}
			if (line[i] == '<' && line[i + 1] == '<') // heredoc
			{
				// printf("sign: <<\n");
				handle_heredoc(filename);
				i++;
			}
			else if (line[i] == '>' && line[i + 1] == '>') // append >>
			{
				// printf("sign: >>\n");
				file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (file_fd == -1)
				{
					perror("open");
					*status = 1;
					break ;
				}
				dup2(file_fd, STDOUT_FILENO);
				close(file_fd);
				i++;
			}
			else if (line[i] == '>') // truncate >
			{
				// printf("sign: >\n");
				file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (file_fd == -1)
				{
					perror("open");
					*status = 1;
					break ;
				}
				dup2(file_fd, STDOUT_FILENO);
				close(file_fd);
			}
			else if (line[i] == '<') // input <
			{
				// printf("sign: <\n");
				file_fd = open(filename, O_RDONLY);
				if (file_fd == -1)
				{
					perror("open");
					*status = 1;
					break ;
				}
				dup2(file_fd, STDIN_FILENO);
				close(file_fd);
			}
			free(filename);
		}
		i++;
	}
	// if (*status == 1)
	// {
	// 	dup2(std.saved_stdin, STDIN_FILENO);
	// 	dup2(std.saved_stdout, STDOUT_FILENO);
	// }
	// close(std.saved_stdin);
	// close(std.saved_stdout);
}

void	join_part(char **s1, char *s2)
{
	int		lens1;
	int		lens2;
	char	*temp;
	int		i;

	lens1 = 0;
	i = 0;
	// fprintf(stderr, "s1: '%s'\ts2: '%s'\n", *s1, s2);
	if (*s1 != NULL)
		lens1 = ft_strlen(*s1);
	if (s2 == NULL)
		return ;
	lens2 = ft_strlen(s2);
	// fprintf(stderr, "lens1: %d\tlens2: %d\n", lens1, lens2);
	temp = (char *)malloc((lens1 + lens2 + 1) * sizeof(char));
	if (!temp)
	{
		perror("Failed to allocate memory for temp in join part");
		exit(EXIT_FAILURE);
	}
	while (*s1 && i < lens1)
	{
		temp[i] = (*s1)[i];
		// fprintf(stderr, "temp[%i] in while cycle: '%c'\n", i, temp[i]);
		i++;
	}
	// write(1, "1passed1\n", 9);
	// fprintf(stderr, "temp after s1 copy: %s\n", temp);
	i = 0;
	while (s2 && i < lens2)
	{
		temp[lens1 + i] = s2[i];
		i++;
	}
	// write(1, "1passed2\n", 9);
	// fprintf(stderr, "temp after s2 copy: %s\n", temp);
	// fprintf(stderr, "lens1 + i: %d\t lens1 + lens2 + 1: %d\n", lens1 + i, lens1 + lens2 + 1);
	temp[lens1 + i] = '\0';
	free(*s1);
	free(s2);
	s2 = NULL;
	*s1 = temp;
	// fprintf(stderr, "s1 in join_part: %s\n", *s1);
}

void	redir_part(char *cmd, int *i)
{
	while (cmd[*i] && (cmd[*i] == '<' || cmd[*i] == '>' || ft_isspace(cmd[*i])))
		(*i)++;
	while (cmd[*i] && !ft_isspace(cmd[*i]))
		(*i)++;
}

char	*before_red(char *cmd, int *i)
{
	int		start;
	int		j;
	char	*bef_red;

	start = *i;
	j = 0;
	while (cmd[*i] && cmd[*i] != '>' && cmd[*i] != '<')
		(*i)++;
	// printf("*i == %d\n", *i);
	// printf("ch == %c\n", cmd[*i]);
	if (start == *i)
		return (NULL);
	// if (start + 1 == *i || *i + 1 == start || start == *i)
	// 	return NULL;
	// printf("*i - start == %d\n", *i - start);
	bef_red = malloc((*i - start + 1));
	if (!bef_red)
	{
		perror("Failed to allocate memory for bef_red");
		exit(EXIT_FAILURE);
	}
	while (start < *i)
	{
		bef_red[j] = cmd[start];
		start++;
		j++;
	}
	bef_red[j] = '\0';
	return (bef_red);
}

char	*remove_redirects(char *cmd)
{
	char	*clean_cmd;
	char	*temp;
	int		i;

	i = 0;
	clean_cmd = NULL;
	// write(1, "passed1\n", 8);
	while (cmd[i])
	{
		// write(1, "passed2\n", 8);
		temp = before_red(cmd, &i);
		// if (!temp)
		// {
		// 	printf("clean_cmd is null\n");
		// 	break ;
		// }
		// write(1, "passed3\n", 8);
		join_part(&clean_cmd, temp);
		// write(1, "passed4\n", 8);
		redir_part(cmd, &i);
	}
	// printf("clean_cmd in remove redirects: %s, %d\n", clean_cmd, ft_strlen(clean_cmd));
	return (clean_cmd);
}


// void	handle_redirection(char *cmd, int *status)
// {
// 	int		file_fd;
// 	char	*filename;

// 	file_fd = -1;
// 	filename = get_filename(cmd);
// 	if (!filename)
// 		return ;
// 	if (strstr(cmd, "<<") && !strstr(cmd, "<<<"))
// 	{
// 		// printf("redirection for <<\n");
// 		handle_heredoc(filename);
// 	}
// 	else if ((strstr(cmd, ">>")))
// 	{
// 		// printf("redirection for >>\n");
// 		file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (file_fd == -1)
// 			return (perror("open"), exit(1));
// 		dup2(file_fd, STDOUT_FILENO);
// 		close(file_fd);
// 	}
// 	else if ((strchr(cmd, '>')))
// 	{
// 		// printf("redirection for >\n");
// 		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (file_fd == -1)
// 			return (perror("open"), exit(1));
// 		dup2(file_fd, STDOUT_FILENO);
// 		close(file_fd);
// 	}
// 	else if ((strchr(cmd, '<')))
// 	{
// 		file_fd = open(filename, O_RDONLY);
// 		if (file_fd == -1)
// 		{
// 			perror("open");
// 			*status = 1;
// 			return ;
// 		}
// 		dup2(file_fd, STDIN_FILENO);
// 		close(file_fd);
// 	}
// 	free(filename);
// }

// char	*remove_redirects(char *cmd)
// {
// 	char	*clean_cmd;
// 	char	*temp;
// 	int		start;
// 	int		end;
// 	int		i;
// 	int		j;

// 	j = 0;
// 	start = 0;
// 	while (cmd[start] && cmd[start] != '>' && cmd[start] != '<')
// 		start++;     // Находим первый символ редиректа
// 	if (!cmd[start]) // Если редиректов нет, просто копируем строку
// 		return (strdup(cmd));
// 	end = start;
// 	while (cmd[end] && (cmd[end] == '>' || cmd[end] == '<'
// 			|| isspace(cmd[end])))
// 		end++;                             // Пропускаем пробелы и сам редирект
// 	while (cmd[end] && !isspace(cmd[end])) // Пропускаем имя файла
// 		end++;
// 	i = 0;
// 	while (cmd[i] && i < start) // Копируем всё до редиректа
// 		i++;
// 	while (cmd[end] && isspace(cmd[end])) // Пропускаем лишние пробелы после имени файла
// 		end++;
// 	clean_cmd = malloc(strlen(cmd) - (end - start) + 1);
// 	if (!clean_cmd)
// 		return (NULL);
// 	j = 0;
// 	i = 0;
// 	while (cmd[i] && i < start) // Копируем всё до редиректа
// 		clean_cmd[j++] = cmd[i++];
// 	while (cmd[end] && cmd[end] != '>' && cmd[end] != '<')
// 		clean_cmd[j++] = cmd[end++];
// 	clean_cmd[j] = '\0';
// 	// printf("clean_cmd in remove redirects: %s\n", clean_cmd);
// 	return (clean_cmd);
// }