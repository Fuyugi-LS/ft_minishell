/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 02:48:01 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/03 03:03:13 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * gnl_strchr - Locate first occurrence of character in string
 * @s: String to search
 * @c: Character to find (converted to char)
 *
 * Return: Pointer to first occurrence of @c in @s, or NULL if not found.
 * If @c is '\0', returns pointer to string terminator.
 */
char	*gnl_strchr(const char *s, int c)
{
	char	fchar;

	if (!s)
		return (NULL);
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

/**
 * gnl_strlen - Compute the length of a string
 * @s: Null-terminated string
 *
 * Return: Number of characters in @s (excluding the null terminator)
 */
size_t	gnl_strlen(const char *s)
{
	const char	*i;

	if (!s)
		return (0);
	i = s;
	while (*i)
		i++;
	return ((size_t)(i - s));
}

/**
 * gnl_strdup - Duplicate a string by allocating memory
 * @str: Source string to duplicate
 *
 * Allocates memory and copies @str into it, including the null terminator.
 *
 * Return: Pointer to the duplicated string, or NULL on allocation failure
 */
char	*gnl_strdup(const char *str)
{
	size_t	i;
	size_t	len;
	char	*s;

	if (!str)
		return (NULL);
	len = gnl_strlen(str) + 1;
	s = malloc(len);
	if (s == NULL)
		return (NULL);
	i = -1;
	while (++i < len)
		s[i] = str[i];
	return (s);
}

/**
 * gnl_substr - Extract a substring from a string
 * @s: Source string
 * @start: Starting index for substring
 * @len: Maximum length of substring
 *
 * If @start is past the end of @s, returns an empty string.
 * Allocates memory for the substring and null terminates it.
 *
 * Return: Pointer to the substring, or NULL on allocation failure
 */
char	*gnl_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	srclen;
	char	*str;

	if (!s)
		return (NULL);
	srclen = gnl_strlen(s);
	if (start > srclen)
		return (gnl_strdup(""));
	if (start + len > srclen)
		len = srclen - start;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[start + i] && i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/**
 * gnl_strjoin - Concatenate two strings into a newly allocated string
 * @a: First string
 * @b: Second string
 *
 * Allocates memory for the concatenated string and copies @a followed by @b.
 *
 * Return: Pointer to the new string, or NULL on allocation failure
 */
char	*gnl_strjoin(char const *a, char const *b)
{
	char	*out;
	int		s;
	int		e;
	size_t	len;

	if (!a || !b)
		return (NULL);
	len = gnl_strlen(a) + gnl_strlen(b) + 1;
	out = malloc(len);
	if (!out)
		return (NULL);
	s = -1;
	while (a[++s])
		out[s] = a[s];
	e = -1;
	while (b[++e])
		out[s + e] = b[e];
	out[s + e] = '\0';
	return (out);
}
