/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omalovic <omalovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:22:01 by tfarkas           #+#    #+#             */
/*   Updated: 2025/04/16 11:52:42 by omalovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*ft_realloc(void *oldptr, size_t oldsize, size_t newsize)
{
	void			*newptr;
	unsigned char	*bytenew;
	unsigned char	*byteold;
	size_t			i;

	i = 0;
	if (newsize == 0)
	{
		free(oldptr);
		return (NULL);
	}
	newptr = (void *)malloc(newsize);
	if (!newptr)
		return (NULL);
	if (!oldptr)
		return (newptr);
	byteold = (unsigned char *)oldptr;
	bytenew = (unsigned char *)newptr;
	while (i < newsize && i < oldsize)
	{
		bytenew[i] = byteold[i];
		i++;
	}
	free(oldptr);
	return (newptr);
}

char	*str_realloc(char *oldstr, size_t newsize)
{
	char	*newstr;
	size_t	oldsize;

	if (oldstr && oldstr[0] != '\0')
		oldsize = (size_t)ft_strlen(oldstr);
	else
		oldsize = 0;
	newstr = (char *)ft_realloc(oldstr, oldsize, newsize);
	if (!newstr || newsize < 1)
		return (NULL);
	newstr[newsize - 1] = '\0';
	return (newstr);
}
