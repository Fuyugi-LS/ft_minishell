/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:08:01 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:08:03 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

/**
 * format_check - process printf format specifier
 * @fd: file descriptor
 * @a: format character
 * @args: arguments list
 * @return: number of characters printed
 */
static int	format_check__fprintf(int fd, char a, void **args, int *arg_idx)
{
	if (a == 'c')
		return (format_char__fprintf(fd, (char)(intptr_t)args[(*arg_idx)++]));
	else if (a == 's')
		return (format_str__fprintf(fd, (char *)args[(*arg_idx)++]));
	else if (a == 'p')
		return (format_ptr__fprintf(fd, args[(*arg_idx)++]));
	else if (a == 'd' || a == 'i')
		return (format_int__fprintf(fd, (int)(intptr_t)args[(*arg_idx)++]));
	else if (a == 'u')
		return (format_uint__fprintf(fd, (unsigned int)(uintptr_t)args[
				(*arg_idx)++]));
	else if (a == 'x')
		return (format_hex__fprintf(fd, (unsigned int)(uintptr_t)args[
				(*arg_idx)++], 0));
	else if (a == 'X')
		return (format_hex__fprintf(fd, (unsigned int)(uintptr_t)args[
				(*arg_idx)++], 1));
	else if (a == '%')
		return (format_pct__fprintf(fd));
	else
	{
		write(fd, "%", 1);
		write(fd, &a, 1);
		return (2);
	}
}

/**
 * ft_fprintf - formatted output to file descriptor
 * @fd: file descriptor
 * @inp: format string
 * @args: array of void pointers (arguments)
 * @return: total characters printed, or -1 on error
 */
int	ft_fprintf(int fd, const char *inp, void **args)
{
	int		count;
	int		arg_idx;

	if (!inp)
		return (-1);
	count = 0;
	arg_idx = 0;
	while (*inp)
	{
		while (*inp)
		{
			if (*inp == '%')
				count += format_check__fprintf(fd, *++inp, args, &arg_idx);
			else
			{
				write(fd, inp, 1);
				count ++;
			}
			inp++;
		}
	}
	return (count);
}
