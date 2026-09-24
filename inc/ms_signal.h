/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vasukmua <vasukmua@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 20:00:00 by vasukmua          #+#    #+#             */
/*   Updated: 2026/09/24 20:00:00 by vasukmua         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_SIGNAL_H
# define MS_SIGNAL_H

# include <signal.h>

extern volatile sig_atomic_t	g_signal;

void	init_signals(void);
void	signals_child_reset(void);
void	signals_heredoc_mode(void);
void	signals_ignore(void);

#endif
