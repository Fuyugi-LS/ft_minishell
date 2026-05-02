/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:08:01 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:08:03 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"
#include <stdarg.h>
#include "libft.h"

static void	append_str(char *buf, int *pos, const char *s)
{
	while (s && *s && *pos < 4095)
		buf[(*pos)++] = *s++;
}

static void	append_int(char *buf, int *pos, long n)
{
	char	*s;

	s = ft_itoa((int)n);
	append_str(buf, pos, s);
	free(s);
}

int	ft_fprintf(int fd, const char *inp, void **args)
{
	char	buf[4096];
	int		pos;
	int		arg_idx;

	pos = 0;
	arg_idx = 0;
	ft_memset(buf, 0, sizeof(buf));
	while (*inp && pos < 4095)
	{
		if (*inp == '%' && inp[1])
		{
			inp++;
			if (*inp == 's')
				append_str(buf, &pos, (char *)args[arg_idx++]);
			else if (*inp == 'd' || *inp == 'i')
				append_int(buf, &pos, (long)(intptr_t)args[arg_idx++]);
			else if (*inp == '%')
				buf[pos++] = '%';
			inp++;
		}
		else
			buf[pos++] = *inp++;
	}
	write(fd, buf, pos);
	return (pos);
}
