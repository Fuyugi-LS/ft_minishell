/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 13:24:51 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 17:32:10 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;

	if (n == 0)
		return (0);
	p1 = (const unsigned char *) s1;
	p2 = (const unsigned char *) s2;
	while (n-- > 1 && *p1 == *p2)
	{
		p1++;
		p2++;
	}
	return (*p1 - *p2);
}
