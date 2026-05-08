/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 18:00:00 by nsongsit          #+#    #+#             */
/*   Updated: 2026/04/20 18:00:00 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H

# include "arena_struct.h"

# define ARENA_BLOCK_SIZE 4096

t_mem_arena	*arena_init(size_t size);
void		*arena_alloc(t_mem_arena **arena, size_t size);
void		arena_reset(t_mem_arena *arena);
void		arena_destroy(t_mem_arena *arena);

#endif
