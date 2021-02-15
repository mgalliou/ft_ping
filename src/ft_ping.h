/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 14:34:57 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/15 23:42:41 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include <netdb.h>

typedef struct	s_ping_env
{
	char	*host;
	int		sockfd;
	int 	sokbuf_size;	
}				t_ping_env;

extern t_ping_env penv;

int build_addrinfo(struct addrinfo **res);
int setup_socket(struct addrinfo *res);
u_short in_cksum(u_short *addr, int len);

#endif
