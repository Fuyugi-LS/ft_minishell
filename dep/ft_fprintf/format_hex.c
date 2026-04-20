/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:07:50 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:07:51 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

/**
 * put_hex_low - print unsigned int as lowercase hex
 * @fd: file descriptor to write out
 * @n: number to convert and print
 */
static void	put_hex_low__fprintf(int fd, unsigned int n)
{
	char	*hex;

	hex = "0123456789abcdef";
	if (n >= 16)
		put_hex_low__fprintf(fd, n / 16);
	write(fd, &hex[n % 16], 1);
}

/**
 * put_hex_up - print unsigned int as uppercase hex
 * @fd: file descriptor to write out
 * @n: number to convert and print
 */
static void	put_hex_up__fprintf(int fd, unsigned int n)
{
	char	*hex;

	hex = "0123456789ABCDEF";
	if (n >= 16)
		put_hex_up__fprintf(fd, n / 16);
	write(fd, &hex[n % 16], 1);
}

/**
 * format_hex - format and print unsigned int as hex
 * @fd: file descriptor to write out
 * @n: number to convert and print
 * @caps: flag for uppercase (1) or lowercase (0)
 * @return: number of characters printed
 */
int	format_hex__fprintf(int fd, unsigned int n, int caps)
{
	int	len;

	if (n == 0)
		return (write(fd, "0", 1));
	len = 0;
	if (caps)
		put_hex_up__fprintf(fd, n);
	else
		put_hex_low__fprintf(fd, n);
	while (n && ++len)
		n /= 16;
	return (len);
}
