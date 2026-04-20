/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:07:35 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:07:37 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

/**
 * format_str - format and print string from args
 * @fd: file descriptor
 * @str: string to print
 * @return: number of characters printed
 */
int	format_str__fprintf(int fd, char *str)
{
	int		len;

	if (!str)
	{
		put_nstr__fprintf(fd, "(null)", 6);
		return (6);
	}
	len = str_len__fprintf(str);
	put_nstr__fprintf(fd, str, len);
	return (len);
}

/**
 * format_char - format and print single character
 * @fd: file descriptor
 * @c: character to print
 * @return: always 1 (characters printed)
 */
int	format_char__fprintf(int fd, char c)
{
	put_char__fprintf(fd, c);
	return (1);
}

/**
 * format_pct - print percent sign
 * @fd: file descriptor
 * @return: always 1 (characters printed)
 */
int	format_pct__fprintf(int fd)
{
	put_char__fprintf(fd, '%');
	return (1);
}
