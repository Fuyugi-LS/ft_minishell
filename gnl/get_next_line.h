/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 02:47:52 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/03 02:47:53 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

char	*get_next_line(int fd);
char	*gnl_strjoin(char const *s1, char const *s2);
char	*gnl_strchr(const char *str, int n);
size_t	gnl_strlen(const char *str);
char	*gnl_strdup(const char *str);
char	*gnl_substr(char const *s, unsigned int start, size_t len);
void	gnl_cleanup(void);
#endif
