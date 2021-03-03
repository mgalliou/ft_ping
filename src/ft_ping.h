/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 14:34:57 by mgalliou          #+#    #+#             */
/*   Updated: 2021/03/03 13:27:29 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include <netdb.h>
#include <netinet/ip_icmp.h>

#define ICMP_DATALEN 56

extern struct ping g_p;

struct					ping
{
	char				*host;
	struct addrinfo 	*ai;
	int					nsent;
	int  				nrcvd;
	int					nerror;
	struct timeval		start;
	int					alrm;
};

int		build_addrinfo(struct addrinfo **ai, char *host);
int		setup_socket(struct addrinfo *ai);
void	build_icmp(struct icmp *icmp, int len);
void	ping_loop(int sockfd, struct addrinfo *ai);
u_short in_cksum(u_short *addr, int len);
int		print_packet(char *buf, int msglen);

#endif
