/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena_struct.h"
#include "ft_fprintf.h"
#include <stdlib.h>

/**
 * arena_init - Initialize a new arena block
 * @size: The size of the block to allocate
 * 
 * Return: A pointer to the initialized arena, or NULL on failure
 */
t_mem_arena	*arena_init(size_t size)
{
	t_mem_arena	*arena;

	arena = malloc(sizeof (t_mem_arena));
	if (!arena)
	{
		ft_fprintf(2, "Error: malloc failed\n", NULL);
		return (NULL);
	}
	arena->block = malloc(size);
	if (!arena->block)
	{
		ft_fprintf(2, "Error: malloc failed\n", NULL);
		free(arena);
		arena = NULL;
		return (NULL);
	}
	arena->size = size;
	arena->offset = 0;
	arena->next = NULL;
	return (arena);
}

static void	*grow_arena(t_mem_arena *curr, size_t size)
{
	void	*ptr;

	if (4096 > size)
		curr->next = arena_init(4096);
	else
		curr->next = arena_init(size);
	if (!curr->next)
		return (NULL);
	curr = curr->next;
	ptr = (char *)curr->block + curr->offset;
	curr->offset += size;
	return (ptr);
}

void	*arena_alloc(t_mem_arena **arena, size_t size)
{
	t_mem_arena	*curr;
	void		*ptr;

	if (!arena || !*arena)
		return (NULL);
	curr = *arena;
	while (curr)
	{
		if (curr->offset + size <= curr->size)
		{
			ptr = (char *)curr->block + curr->offset;
			curr->offset += size;
			return (ptr);
		}
		if (!curr->next)
			break ;
		curr = curr->next;
	}
	return (grow_arena(curr, size));
}

/**
 * arena_reset - Reset the arena (pseudo-free)
 * @arena: The arena to reset
 */
void	arena_reset(t_mem_arena *arena)
{
	t_mem_arena	*curr;

	curr = arena;
	while (curr)
	{
		curr->offset = 0;
		curr = curr->next;
	}
}

/**
 * arena_destroy - Truly free the arena when shell exits
 * @arena: The arena to destroy
 */
void	arena_destroy(t_mem_arena *arena)
{
	t_mem_arena	*curr;
	t_mem_arena	*next;

	curr = arena;
	while (curr)
	{
		next = curr->next;
		if (curr->block)
		{
			free(curr->block);
			curr->block = NULL;
		}
		free(curr);
		curr = NULL;
		curr = next;
	}
}
