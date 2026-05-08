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
			free(*buffer);
			*buffer = NULL;
			return (NULL);
		}
		if (rdbytes == 0)
			return (eoljoin(rdbytes, buffer));
		rdreturn[rdbytes] = 0;
		tmp = gnl_strjoin(*buffer, rdreturn);
		free(*buffer);
		*buffer = tmp;
		nl = gnl_strchr(*buffer, '\n');
	}
	return (eoljoin(nl - *buffer + 1, buffer));
}

static char	**gnl_buf(void)
{
	static char	*buf[1024];

	return (buf);
}

char	*get_next_line(int fd)
{
	char	**b;
	char	*rdreturn;
	char	*out;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 1024)
		return (NULL);
	rdreturn = malloc(BUFFER_SIZE + 1);
	if (!rdreturn)
		return (NULL);
	b = gnl_buf();
	if (!b[fd])
		b[fd] = gnl_strdup("");
	out = rdline(fd, &b[fd], rdreturn);
	free(rdreturn);
	return (out);
}

void	gnl_cleanup(void)
{
	char	**b;
	int		i;

	b = gnl_buf();
	i = 0;
	while (i < 1024)
	{
		if (b[i])
		{
			free(b[i]);
			b[i] = NULL;
		}
		i++;
	}
}
