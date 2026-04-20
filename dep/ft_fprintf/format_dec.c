/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_dec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:08:12 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:08:13 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

/**
 * num_len - count digits in a long integer
 * @n: number to count digits of
 * @return: total number of digits that would be printed
 */
static int	num_len__fprintf(long n)
{
	int		len;
	long	num;
	int		neg;

	len = 0;
	num = n;
	neg = 0;
	if (num == 0)
		return (1);
	if (num < 0)
	{
		num = -(num);
		neg = 1;
	}
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return (len + neg);
}

/**
 * format_int - print long integer recursively
 * @fd: file descriptor to write out
 * @nb: number to print
 * @return: number of characters printed
 */
int	format_int__fprintf(int fd, long nb)
{
	long	n;
	char	tmp;

	n = nb;
	if (n < 0)
	{
		n = (-1) * n;
		write(fd, "-", 1);
	}
	if (n < 10)
	{
		tmp = n + '0';
		write(fd, &tmp, 1);
	}
	else
	{
		format_int__fprintf(fd, n / 10);
		tmp = n % 10 + '0';
		write(fd, &tmp, 1);
	}
	return (num_len__fprintf(nb));
}
