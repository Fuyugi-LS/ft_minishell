/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/05/08 06:07:33 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static void	write_escaped_value(char *val)
{
	int	i;

	i = 0;
	while (val[i])
	{
		if (val[i] == '"' || val[i] == '\\')
			write(1, "\\", 1);
		write(1, val + i, 1);
		i++;
	}
}

static void	write_export_line(char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	write(1, "declare -x ", 11);
	if (eq)
	{
		write(1, entry, eq - entry);
		write(1, "=\"", 2);
		write_escaped_value(eq + 1);
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, entry, ft_strlen(entry));
		write(1, "\n", 1);
	}
}

static void	sort_env_strings(char **arr, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (++i < len - 1)
	{
		j = i;
		while (++j < len)
		{
			if (ft_strncmp(arr[i], arr[j], 1024) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void	print_exported(char **envp)
{
	int		len;
	char	**sorted;
	int		i;

	len = 0;
	while (envp[len])
		len++;
	sorted = malloc(sizeof (char *) * (len + 1));
	i = -1;
	while (++i < len)
		sorted[i] = envp[i];
	sorted[len] = NULL;
	sort_env_strings(sorted, len);
	i = -1;
	while (sorted[++i])
		write_export_line(sorted[i]);
	free(sorted);
}
