/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 14:34:57 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 11:50:10 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include <netdb.h>
#include <netinet/ip.h>

typedef struct	s_ping_env
{
	char	*host;
	int		sockfd;
	int 	sokbuf_size;	
	char	packet[IP_MAXPACKET];
}				t_ping_env;

extern t_ping_env g_p;

int		build_addrinfo(struct addrinfo **res);
int		setup_socket(struct addrinfo *res);
int		send_packet(struct addrinfo *ai);
u_short in_cksum(u_short *addr, int len);
int		recv_packet(struct addrinfo **ai);

#endif
