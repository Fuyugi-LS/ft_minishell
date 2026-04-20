/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_ptr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:07:45 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:07:46 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

/**
 * put_str - write string to stdout
 * @fd: file descriptor
 * @str: null-terminated string to print
 */
static void	put_str__fprintf(int fd, const char *str)
{
	while (*str)
		write(fd, str++, 1);
}

/**
 * put_ptr_hex - convert number to hexadecimal string
 * @num: number to convert
 * @hex: buffer to store hexadecimal digits
 * @i: pointer to current position in buffer
 */
static void	put_ptr_hex__fprintf(uintptr_t num, char *hex, int *i)
{
	int	digit;

	while (num > 0)
	{
		digit = num % 16;
		if (digit < 10)
			hex[(*i)++] = digit + '0';
		else
			hex[(*i)++] = (digit - 10) + 'a';
		num /= 16;
	}
}

/**
 * format_ptr - format and print pointer value
 * @fd: file descriptor
 * @ptr: pointer to format
 * @return: number of characters printed
 */
int	format_ptr__fprintf(int fd, void *ptr)
{
	uintptr_t	num;
	char		hex[16];
	int			i;
	int			len;

	i = 0;
	num = (uintptr_t)ptr;
	if (ptr == NULL)
	{
		put_str__fprintf(fd, "(nil)");
		return (5);
	}
	put_str__fprintf(fd, "0x");
	put_ptr_hex__fprintf(num, hex, &i);
	len = i;
	while (i--)
		write(fd, &hex[i], 1);
	return (len + 2);
}
