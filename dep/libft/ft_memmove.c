/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:41:33 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 12:36:44 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*a;
	const unsigned char	*b;

	i = 0;
	a = (unsigned char *) dest;
	b = (const unsigned char *) src;
	if (dest < src + n && dest >= src)
	{
		while (n--)
			a[n] = b[n];
	}
	else
	{
		while (i < n)
		{
			a[i] = b[i];
			i += 1;
		}
	}
	return (dest);
}
