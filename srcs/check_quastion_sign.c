/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_quastion_sign.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 13:02:47 by omalovic          #+#    #+#             */
/*   Updated: 2025/04/16 14:29:42 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_to_exit_status(int i, char **line, char *status)
{
	char	*result;
	int		index;
	int		j;
	int		rest;

	result = malloc(strlen(*line) - 2 + strlen(status) + 1);
	if (!result)
		return ;
	index = 0;
	j = 0;
	while (index < i)
	{
		result[index] = (*line)[index];
		index++;
	}
	rest = index;
	while (status[j])
		result[index++] = status[j++];
	rest += 2;
	while ((*line)[rest])
		result[index++] = (*line)[rest++];
	result[index] = '\0';
	free(*line);
	*line = result;
}

int	check_quastion_sign(char **line, int status)
{
	int		i;
	int		flag_single;
	int		flag_double;
	char	*ch_status;

	i = 0;
	flag_single = 0;
	flag_double = 0;
	ch_status = ft_itoa(status);
	if (!ch_status)
		return (1);
	while ((*line)[i])
	{
		if ((*line)[i] == '\'' && flag_double == 0)
			flag_single = !flag_single;
		else if ((*line)[i] == '\"' && flag_single == 0)
			flag_double = !flag_double;
		if ((*line)[i] == '$' && flag_single == 0)
		{
			if ((*line)[i + 1] == '?')
				change_to_exit_status(i, line, ch_status);
		}
		i++;
	}
	return (free(ch_status), 0);
}

// int	copy_before_status(char *dst, char *src, int stop)
// {
// 	int	i;

// 	i = 0;
// 	while (i < stop)
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	return (i);
// }

// int	insert_status(char *dst, int index, char *status)
// {
// 	int	j;

// 	j = 0;
// 	while (status[j])
// 		dst[index++] = status[j++];
// 	return (index);
// }

// void	change_to_exit_status(int i, char **line, char *status)
// {
// 	char	*result;
// 	int		index;
// 	int		rest;

// 	result = malloc(strlen(*line) - 2 + strlen(status) + 1);
// 	if (!result)
// 		return ;
// 	index = copy_before_status(result, *line, i);
// 	index = insert_status(result, index, status);
// 	rest = i + 2;
// 	while ((*line)[rest])
// 		result[index++] = (*line)[rest++];
// 	result[index] = '\0';
// 	free(*line);
// 	*line = result;
// }

// int	check_quastion_sign(char **line, int wstatus)
// {
// 	int		i;
// 	int		flag_single;
// 	int		flag_double;
// 	char	*status;

// 	i = 0;
// 	flag_single = 0;
// 	flag_double = 0;
// 	status = ft_itoa(wstatus);
// 	if (!status)
// 		exit(1);
// 	while ((*line)[i])
// 	{
// 		if ((*line)[i] == '\'' && flag_double == 0)
// 			flag_single = !flag_single;
// 		else if ((*line)[i] == '\"' && flag_single == 0)
// 			flag_double = !flag_double;
// 		if ((*line)[i] == '$' && flag_single == 0)
// 		{
// 			if ((*line)[i + 1] == '?')
// 				change_to_exit_status(i, line, status);
// 			i = -1;
// 		}
// 		i++;
// 	}
// 	return (free(status), 0);
// }

// int	check_quastion_sign(char **line, char *status)
// {
// 	int	i;
// 	int	flag;

// 	flag = 0;
// 	i = 0;
// 	while ((*line)[i])
// 	{
// 		if ((*line)[i] == '\'' && flag == 0)
// 			flag = 1;
// 		else if ((*line)[i] == '\'' && flag == 1)
// 			flag = 0;
// 		if ((*line)[i] == '$' && flag == 0)
// 		{
// 			if ((*line)[i + 1] == '?')
// 			{
// 				change_to_exit_status(i, line, status);
// 				i = -1;
// 			}
// 		}
// 		i++;
// 	}
// 	free(status);
// 	return (0);
// }

// int main()
// {
// 	char *str1 = strdup("Hello, world! \'$?\'");
// 	char *num = strdup("255");
// 	check_quastion_sign(&str1, num);
// 	printf("%s\n", str1);
// }