/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:10:26 by alex              #+#    #+#             */
/*   Updated: 2025/04/08 17:33:39 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_cd(char *line)
{
	char	**arr;
	char	*path;
	int 	i;
	char	*clean_path;

	clean_path = NULL;
	i = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (free(line), exit(1), 1);
	while (arr[i])
		i++;
	if (i > 2)
	{
		printf("minishell: cd: too many arguments\n");
		free_arr(arr);
		return (1);
	}
	if (i == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			free_arr(arr);
			return (1);
		}
	}
	else
	{
		clean_path = remove_quotes(arr[1]);
		if (!clean_path)
			return (free_arr(arr), 1);
		// path = arr[1];
		path = clean_path;
	}
	if (chdir(path) != 0)
		return (perror("minishell: cd"), 1);
	free_arr(arr);
	if (clean_path)
		free(clean_path);
	return (0);
}
