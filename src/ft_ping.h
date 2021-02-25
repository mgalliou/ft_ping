/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 14:34:57 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/25 16:20:03 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include <netdb.h>
#include <netinet/ip_icmp.h>

#define ICMP_DATALEN 56

struct				ping
{
	char			*host;
	int				nsent;
	int  			nrcvd;
	struct timeval	start;
};

int		build_addrinfo(struct addrinfo **ai, char *host);
int		setup_socket(struct addrinfo *ai);
int		send_packet(int sockfd, struct icmp *icmp, struct addrinfo *ai);
u_short in_cksum(u_short *addr, int len);
int		recv_packet(int sockfd, struct msghdr *msghdr);
int		print_packet(char *buf, int msglen);

#endif
