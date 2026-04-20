/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:36:49 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 15:41:30 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*result;
	size_t	len;
	size_t	i;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	result = (char *)ft_calloc(len + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = f((unsigned int)i, s[i]);
		i++;
	}
	return (result);
}
