/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 18:14:33 by alex              #+#    #+#             */
/*   Updated: 2025/03/28 12:01:25 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int get_var_name_size(char *str)
{
	int start = 0;
	int end = 0;
	int	flag = 0;

	while (str[start])
	{
		if ((str)[start] == '\'' && flag == 0)
			flag = 1;
		else if ((str)[start] == '\'' && flag == 1)
			flag = 0;
		if (str[start] == '$' && flag == 0)
			break ;
		start++;
	}
	if (str[start] != '$')
		return 0;
	start++;
	end = start;
	while (str[end] && str[end] != ' ' && str[end] != '$' && str[end] != '=' && str[end] != '\0')
		end++;
	return (end - start);
}

void get_var_name(char *dest, char *str)
{
	int start = 0;
	int end = 0;
	int i = 0;
	int flag = 0;

	while (str[start] != '\0' && str[start] != '$')
	{
		start++;
	}
	if (str[start] != '$')
		return ;
	start++;
	end = start;
	while (str[end] && str[end] != ' ' && str[end] != '\t' && str[end] != '\0' && str[end] != '$' && str[end] != '=')
		end++;
	while (start < end)
	{
		dest[i] = str[start];
		i++;
		start++;
	}
	dest[i] = '\0';
}

int cmp_names(char *name1, char *name2)
{
	int i = 0;

	while (name1[i] && name2[i] && name1[i] != '=' && name2[i] != '=')
	{
		if (name1[i] != name2[i])
			return 0;
		i++;
	}
	if ((name1[i] == '=' && name2[i] == '\0') || (name1[i] == '\0' && name2[i] == '\0'))
		return 1;
	return 0;
}

char *get_name(char *str)
{
	int i = 0;
	char *result;
	int j;

	while (str[i] && str[i] != '=')
		i++;
	result = malloc(i + 1);
	if (!result)
		return NULL;
	j = 0;
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[j] = '\0';
	return result;
}

char *find_var_value(char *var_name)
{
	extern char **environ;
	int i = 0;
	char *value;

	while (environ[i])
	{
		if (cmp_names(environ[i], var_name))
		{
			value = environ[i];
			while (*value && *value != '=')
				value++;
			if (*value == '=')
				return value + 1;
		}
		i++;
	}
	return NULL;
}

void	change_str(char **str, char *name, char *value)
{
	int	i = 0;
	int	start = 0;
	int end = 0;
	char	*result;
	int		len = ft_strlen(*str) - ft_strlen(name) + ft_strlen(value) + 1; // i didnt minus dollar space, cause its space will be now for the '\0'

	result = malloc(len);
	if (!result)
		return ;
	while ((*str)[start] && (*str)[start] != '$')
	{
		result[start] = (*str)[start];
		start++;
	}
	end = start; // запомнить индекс для result
	start++; // пропускаем $
	while (i < ft_strlen(value)) // переписываем value
	{
		result[end] = value[i];
		end++;
		i++;
	}
	result[end] = '\0';
	// printf("%s %d\n", result, ft_strlen(result));
	i = end; // теперь индекс для result
	while ((*str)[start] && (*str)[start] != ' ' && (*str)[start] != '$' && (*str)[start] != '=' && (*str)[start] != '\0' && (*str)[start] != '\t')
		start++;
	// printf("%d\n", start);
	while ((*str)[start] && i < len)
	{
		result[i] = (*str)[start];
		i++;
		start++;
	}
	result[i] = '\0';
	// printf("%s\n", result);
	free(*str);
	*str = result;
}

void	remove_var_name(char **str, char *name)
{
	int	i = 0;
	int	start = 0;
	char	*result;
	int		len = ft_strlen(*str) - ft_strlen(name) + 1;
	
	result = malloc(len);
	if (!result)
		return ;
	while ((*str)[i] && (*str)[i] != '$')
	{
		result[i] = (*str)[i];
		i++;
	}
	start = i;
	i = i + ft_strlen(name) + 1;
	while ((*str)[i])
	{
		result[start] = (*str)[i];
		i++;
		start++;
	}
	result[start] = '\0';
	free(*str);
	*str = result;
}

void	remove_chars(char **str, char ch)
{
	char *new_str = NULL;
	int i = 0;
	int len = 0;
	int j = 0;

	// printf("i'm in func remove_chars\n");
	if (!*str || !str || ch == '\0')
		return ;
	// printf("str before: %s\n", *str);
	while ((*str)[i])
    {
        if ((*str)[i] != ch)
            len++;
        i++;
    }
	new_str = malloc(len + 1);
	if (!new_str)
		return ;
	i = 0;
	while ((*str)[i])
	{
		if ((*str)[i] != ch)
		{
			new_str[j] = (*str)[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	// printf("str after: %s\n", new_str);
	free(*str);
	*str = new_str;
}

int get_len_double_quotes(char **str)
{
    int i = 0;
    int len = 0;
    int flag_single = 0;  // Флаг для одинарных кавычек
    int flag_double = 0;  // Флаг для двойных кавычек

    while ((*str)[i])
    {
        // Переключаем флаг для одинарных кавычек
        if ((*str)[i] == '\'' && flag_double == 0)
            flag_single = !flag_single;
        
        // Переключаем флаг для двойных кавычек
        if ((*str)[i] == '\"' && flag_single == 0)
            flag_double = !flag_double;

        // Увеличиваем длину только для символов, которые не в двойных кавычках
        if ((*str)[i] != '\"' && flag_single == 0)
        {
            len++;
        }
        i++;
    }
    return len;
}

void remove_double_quotes(char **str)
{
    int i = 0;
    int j = 0;
    char *new_str = NULL;
    int flag_single = 0;
    int flag_double = 0;

    new_str = malloc(get_len_double_quotes(str) + 1);
    if (!new_str)
        return ;
    while ((*str)[i])
    {
        if ((*str)[i] == '\'' && flag_double == 0)
            flag_single = !flag_single;
        if ((*str)[i] == '\"' && flag_single == 0)
            flag_double = !flag_double;
        if ((*str)[i] != '\"' || flag_single == 1)
        {
            new_str[j] = (*str)[i];
            j++;
        }
        i++;
    }
    new_str[j] = '\0';
    free(*str);
    *str = new_str;
}

void	bridge_var(char **str)
{
	char *var_name;
	int size;
	char *var_value;

	// printf("str: %s\n", *str);
	// remove_double_quotes(str);
	size = get_var_name_size(*str);
	// printf("size: %d\n", size);
	if (size < 1)
	{
		// return (remove_single_quotes(str));
		return ;
	}
	var_name = malloc(size + 1);
	if (!var_name)
		return ;
	get_var_name(var_name, *str);
	// printf("%s\n", var_name);
	var_value = find_var_value(var_name);
	if (var_value)
	{
		change_str(str, var_name, var_value);
		// printf("%s\n", *str);
		return (free(var_name));
	}
	else
	{
		remove_var_name(str, var_name);
	}
	// printf("Variable not found in environ\n");
	return (free(var_name));
}

// int main()
// {
//     char *str = strdup("echo \'\"$?\"\'mam");  // Можешь заменить на $PS или $PATH
//     bridge_var(&str);
// 	// remove_double_quotes(&str);
// 	printf("%s\n", str);
// 	free(str);
// }

// -L../lib/libft -lft