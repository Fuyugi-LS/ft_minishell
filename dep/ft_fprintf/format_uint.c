/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_uint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:07:33 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:07:34 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

/**
 * format_uint - format and print uint as int
 * @fd: file descriptor to write out
 * @n: int to be printed as uint
 * @return: number of characters printed
 */
int	format_uint__fprintf(int fd, int n)
{
	unsigned long	num;

	if (n >= 0)
		return (format_int__fprintf(fd, n));
	num = (unsigned long)n + 4294967296;
	return (format_int__fprintf(fd, num));
}
