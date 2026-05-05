/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 02:47:56 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/03 03:06:21 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * clear - Free memory and set pointer to NULL
 * @p: Pointer to the memory pointer to free
 *
 * Frees the memory pointed to by @p and sets it to NULL
 */
static void	clear(char **p)
{
	if (*p != NULL)
	{
		free(*p);
		*p = NULL;
	}
}

/**
 * eoljoin - Split buffer at newline or return full buffer
 * @nlptr: Position of the newline character in the buffer
 * @buffer: Pointer to the buffer containing the string
 *
 * If @nlptr is <= 0:
 *   - Returns NULL if buffer is empty
 *   - Returns the entire buffer otherwise
 * If @nlptr > 0:
 *   - Splits buffer at @nlptr
 *   - Returns string up to newline
 *   - Keeps the rest in the buffer
 *
 * Return: String containing one line, or NULL if nothing to return
 */
static char	*eoljoin(ssize_t nlptr, char **buffer)
{
	char	*out;
	char	*tmp;

	tmp = NULL;
	if (nlptr <= 0)
	{
		if (**buffer == '\0')
		{
			free(*buffer);
			*buffer = NULL;
			return (NULL);
		}
		out = *buffer;
		*buffer = NULL;
		return (out);
	}
	tmp = gnl_substr(*buffer, nlptr, BUFFER_SIZE);
	out = *buffer;
	out[nlptr] = 0;
	*buffer = tmp;
	return (out);
}

/**
 * rdline - Read from file descriptor until a newline is found
 * @fd: File descriptor to read from
 * @buffer: Pointer to the static buffer for leftover data
 * @rdreturn: Temporary buffer to read data into
 *
 * Reads chunks from the file descriptor and appends to the buffer
 * until a newline is found or EOF is reached.
 *
 * Return: String containing one line, or NULL on error/EOF
 */
static char	*rdline(int fd, char **buffer, char *rdreturn)
{
	ssize_t	rdbytes;
	char	*tmp;
	char	*nl;

	nl = gnl_strchr(*buffer, '\n');
	tmp = NULL;
	rdbytes = 0;
	while (!nl)
	{
		rdbytes = read(fd, rdreturn, BUFFER_SIZE);
		if (rdbytes == -1)
		{
			clear(buffer);
			return (NULL);
		}
		if (rdbytes == 0)
			return (eoljoin(rdbytes, buffer));
		rdreturn[rdbytes] = 0;
		tmp = gnl_strjoin(*buffer, rdreturn);
		clear(buffer);
		*buffer = tmp;
		nl = gnl_strchr(*buffer, '\n');
	}
	return (eoljoin(nl - *buffer + 1, buffer));
}

/**
 * get_next_line - Get the next line from a file descriptor
 * @fd: File descriptor to read from
 *
 * Allocates a temporary buffer, reads from @fd using rdline,
 * and returns the next line from the static buffer.
 *
 * Return: Next line as a malloc'ed string, or NULL on EOF/error
 */
char	*get_next_line(int fd)
{
	static char	*buffer[1024];
	char		*rdreturn;
	char		*out;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 1024)
		return (NULL);
	rdreturn = malloc(BUFFER_SIZE + 1);
	if (!rdreturn)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = gnl_strdup("");
	out = rdline(fd, &buffer[fd], rdreturn);
	clear(&rdreturn);
	return (out);
}
