/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_types.h"
#include "arena.h"
#include "ft_fprintf.h"

/**
 * cmd_table_new - Allocate an array of flat commands
 * @arena: The memory arena
 * @count: Number of commands in pipeline
 * 
 * Return: A pointer to the zero-initialized command table
 */
t_cmd	*cmd_table_new(struct s_arena **arena, int count)
{
	t_cmd	*table;
	int		i;

	table = arena_alloc(arena, sizeof(t_cmd) * count);
	if (!table)
	{
		ft_fprintf(2, "Error: arena alloc for cmd table failed\n", NULL);
		return (NULL);
	}
	i = -1;
	while (++i < count)
	{
		table[i].args = NULL;
		table[i].infile = NULL;
		table[i].outfile = NULL;
		table[i].append_mode = 0;
		table[i].heredoc_delim = NULL;
	}
	return (table);
}
