/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_arena.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_ARENA_H
# define MS_ARENA_H

# include <stddef.h>

# define ARENA_BLOCK_SIZE 4096

typedef struct s_mem_arena
{
	void				*block;
	size_t				size;
	size_t				offset;
	struct s_mem_arena	*next;
}	t_mem_arena;

t_mem_arena	*arena_init(size_t size);
void		*arena_alloc(t_mem_arena **arena, size_t size);
void		arena_reset(t_mem_arena *arena);
void		arena_destroy(t_mem_arena *arena);

#endif
