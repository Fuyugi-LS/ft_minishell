/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 18:20:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/09/24 18:20:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	print_error(char *before, char *value, char *after)
{
	ft_putstr_fd(before, 2);
	if (value)
		ft_putstr_fd(value, 2);
	ft_putendl_fd(after, 2);
}
