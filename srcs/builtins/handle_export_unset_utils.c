/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_export_unset_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:51:40 by alex              #+#    #+#             */
/*   Updated: 2025/04/19 12:30:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	find_var_in_env(char *name, char **myenvp)
{
	int	i;
	int	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while (myenvp[i])
	{
		if (ft_strncmp(myenvp[i], name, name_len) == 0
			&& (myenvp[i][name_len] == '=' || myenvp[i][name_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	cycle_for_unsetenv(char **arr, char *clean_line,
		char **new_env, char ***myenvp)
{
	int	i;

	i = 1;
	while (arr[i])
	{
		clean_line = remove_quotes(arr[i]);
		if (!clean_line)
			return (free_arr(arr), 1);
		new_env = my_unsetenv(clean_line, myenvp);
		free(clean_line);
		if (!new_env)
		{
			i++;
			continue ;
		}
		free_arr(*myenvp);
		*myenvp = new_env;
		i++;
	}
	return (0);
}

int	handle_unset(char *line, char ***myenvp)
{
	int		i;
	char	**arr;
	char	**new_env;
	char	*clean_line;

	if (!line || !*myenvp)
		return (perror("[handle_unset] malloc/readline"), exit(1), 1);
	arr = ft_split(line, ' ');
	if (!arr)
		return (1);
	if (ft_arrlen(arr) == 1)
		return (write_stderr("unset: not enough arguments"), free_arr(arr), 1);
	if (cycle_for_unsetenv(arr, clean_line, new_env, myenvp))
		return (free_arr(arr), 1);
	return (free_arr(arr), 0);
}

int	process_string_env(char *clean_line, char ***myenvp)
{
	char	*name;
	char	*equals_pos;
	char	*value;

	value = NULL;
	equals_pos = ft_strchr(clean_line, '=');
	if (equals_pos && equals_pos + 1)
	{
		name = ft_substr(clean_line, 0, equals_pos - clean_line);
		if (!name)
			return (1);
		value = ft_strdup(equals_pos + 1);
		if (!value)
			return (free(name), 1);
	}
	else
	{
		name = ft_strdup(clean_line);
		if (!name)
			return (free(name), 1);
	}
	if (mysetenv(name, value, myenvp))
		return (free(name), free(value), 1);
	return (free(name), free(value), 0);
}

int	handle_export(char *line, char ***myenvp)
{
	char	**arr;
	char	*equals_pos;
	char	*value;
	int		i;
	char	*clean_line;

	if (ft_strlen(line) == 6 || check_line(line, 7))
		return (print_env(myenvp));
	arr = ft_split(line, ' ');
	if (!arr)
		return (1);
	i = 1;
	while (arr[i])
	{
		if (ft_strchr(arr[i], ' '))
			return (write_stderr("minishell: bad assignment"),
				free_arr(arr), 1);
		clean_line = remove_quotes(arr[i]);
		if (process_string_env(clean_line, myenvp))
			return (free_arr(arr), free(clean_line), 1);
		free(clean_line);
		i++;
	}
	free_arr(arr);
	return (0);
}
