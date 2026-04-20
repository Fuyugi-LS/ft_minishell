/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:07:41 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:07:42 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

/**
 * str_len - compute string length
 * @str: string to measure
 * @return: number of characters in string
 */
int	str_len__fprintf(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/**
 * put_nstr - print limited string
 * @fd: file descriptor
 * @str: string to print
 * @n: maximum characters to print
 */
void	put_nstr__fprintf(int fd, const char *str, int n)
{
	while (*str && n--)
		write(fd, str++, 1);
}

/**
 * put_char - writes a single character
 * @fd: file descriptor
 * @c: the character to write
 */
void	put_char__fprintf(int fd, char c)
{
	write(fd, &c, 1);
}
