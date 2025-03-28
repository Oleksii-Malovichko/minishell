/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:40:45 by omalovic          #+#    #+#             */
/*   Updated: 2025/03/28 11:00:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	mywrite(char *line, int fd)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(line);
	// if (line[i] == 34)
	// 	i++;
	// if (line[len - 1] == 34)
	// 	len--;
	while (i < len)
	{
		write(fd, &line[i], 1);
		i++;
	}
}

void	show_input(char **arr, int fd, int flag)
{
	int	i;
	int len;

	if (flag)
		i = 2;
	else
		i = 1;
	len = 0;
	while (arr[len])
		len++;
	while (arr[i])
	{
		if (i == len - 1)
		{
			mywrite(arr[i], fd);
			if (!flag)
				write(fd, "\n", 1);
			return ;
		}
		mywrite(arr[i], fd);
		write(fd, " ", 1);
		i++;
	}
}

int	handle_echo(char *line, int fd)
{
	char	**arr;
	int		i;

	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1), 1);
	while (arr[i])
		i++;
	if (i == 1)
	{
		return (write(fd, "\n", 1), free_arr(arr), 0);
	}
	if (i == 2)
	{
		if (ft_strcmp(arr[1], "-n") == 0)
			return (free_arr(arr), 0);
		else
			return (mywrite(arr[1], fd), write(1, "\n", 1), 0);
	}
	if (i > 2)
	{
		if (ft_strcmp(arr[1], "-n") == 0)
			return (show_input(arr, fd, 1), free_arr(arr), 0);
		show_input(arr, fd, 0);
	}
	free_arr(arr);
	return (0);
}

// int main()
// {
// 	handle_echo("echo \"'hello'\"", 1);
// }