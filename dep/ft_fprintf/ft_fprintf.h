/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42bangkok.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:07:56 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/05 00:07:57 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FPRINTF_H
# define FT_FPRINTF_H

# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

/* Main format functions */
int		ft_fprintf(int fd, const char *inp, void **args);
int		format_char__fprintf(int fd, char c);
int		format_str__fprintf(int fd, char *str);
int		format_pct__fprintf(int fd);
int		format_ptr__fprintf(int fd, void *ptr);
int		format_int__fprintf(int fd, long nb);
int		format_uint__fprintf(int fd, int n);
int		format_hex__fprintf(int fd, unsigned int n, int caps);

/* String utilities */
int		str_len__fprintf(const char *str);
void	put_nstr__fprintf(int fd, const char *str, int n);
void	put_char__fprintf(int fd, char c);
#endif
