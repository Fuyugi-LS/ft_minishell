/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:47:56 by vasukmua          #+#    #+#             */
/*   Updated: 2026/04/20 16:32:34 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	fs_isset(char c, const char *set)
{
	while (*set)
		if (*set++ == c)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trimmed;
	size_t	start;
	size_t	end;
	size_t	i;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && fs_isset(s1[start], set))
		start++;
	while (end > start && fs_isset(s1[end - 1], set))
		end--;
	trimmed = (char *)ft_calloc((end - start + 1), sizeof(*s1));
	if (!trimmed)
		return (NULL);
	i = 0;
	while (start < end)
		trimmed[i++] = s1[start++];
	trimmed[i] = '\0';
	return (trimmed);
}
