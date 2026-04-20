/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 14:12:45 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 16:21:38 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	fchar;

	fchar = (char)c;
	while (*s != '\0')
	{
		if (*s == fchar)
			return ((char *) s);
		s += 1;
	}
	if (fchar == '\0')
		return ((char *) s);
	return (NULL);
}
