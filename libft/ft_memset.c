/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 13:52:01 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 13:16:17 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	i;
	unsigned char	*p;

	i = (unsigned char)c;
	p = (unsigned char *)s;
	while (n--)
		p[n] = i;
	return (s);
}
