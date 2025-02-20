/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:15:14 by alex              #+#    #+#             */
/*   Updated: 2025/02/20 18:23:19 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error(void)
{
	perror("Error");
	exit(1);
}

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	ft_getcwd(char *line)
{
	char	buffer[128];
	size_t	size;

	size = 128;
	if (getcwd(buffer, size) == NULL)
	{
		perror("minishell: getcwd");
		free(line);
		exit(1);
	}
	printf("%s\n", buffer);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	handle_cd(char *line)
{
	char	**arr;
	char	*path;
	int 	i;

	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1));
	while (arr[i])
		i++;
	if (i > 2)
	{
		printf("minishell: cd: too many arguments\n");
		free_arr(arr);
		return ;
	}
	if (i == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			free_arr(arr);
			return ;
		}
	}
	else
		path = arr[1];
	if (chdir(path) != 0)
		perror("minishell: cd");
	free_arr(arr);
}

void	print_env(void)
{
	int				i;
	extern	char	**environ;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}

void	execute_cmd(char *line)
{
	pid_t	pid;
	int		status;
	char	*args[] = {line, NULL};
	char	*env[] = {NULL};

	pid = fork();
	if (pid == 0)
	{
		execve("/usr/bin/ls", args, env);
		perror("execve");
		exit(127);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork");
}

int	check_line(char *line, int i)
{
	int	len;

	len = ft_strlen(line);
	while (i < len)
	{
		if (!ft_isspace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

void	choose_cmd(char *line)
{
	if (is_empty(line))
		return ;
	if (ft_strcmp(line, "pwd") == 0)
		ft_getcwd(line);
	else if (ft_strncmp(line, "cd ", 3) == 0 || ft_strcmp(line, "cd") == 0)
		handle_cd(line);
	else if (ft_strncmp(line, "echo ", 5) == 0 || ft_strcmp(line, "echo") == 0)
		handle_echo(line);
	else if (ft_strcmp(line, "env") == 0)
		print_env();
	else if (ft_strncmp(line, "export ", 7) == 0 || ft_strcmp(line, "export") == 0)
		handle_export(line);
	else if (ft_strncmp(line, "unset ", 6) == 0 || ft_strcmp(line, "unset") == 0)
		handle_unset(line);
	else
		execute_cmd(line);
	// else
		// printf("minishell: command not found: %s\n", line);
}

void	disable_ctrl_c_output(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	setup_signal_handlers(void)
{
	struct sigaction sa;

	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

int main(void)
{
	char	*line;

	disable_ctrl_c_output();
	setup_signal_handlers();
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, "exit") == 0)
			break ;
		else if (ft_strcmp(line, "clear") == 0)
			rl_clear_history();
		else
		{
			add_history(line);
			// bridge_var(&line);
			choose_cmd(line);
		}
		free(line);
	}
	rl_clear_history();
	free(line);
}

/* 
найти переменную в строке
выяснить длину значения
выделить память (минус длина названия переменной, плюс длина значения)
переписать массив и начиная с доллара начинать вписывать значение */