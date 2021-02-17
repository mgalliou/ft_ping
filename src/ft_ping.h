/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 14:34:57 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/17 15:56:02 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

#include <netdb.h>
#include <netinet/ip.h>

int		build_addrinfo(struct addrinfo **ai, char *host);
int		setup_socket(struct addrinfo *ai);
int		send_packet(int sockfd, struct addrinfo *ai);
u_short in_cksum(u_short *addr, int len);
int		recv_packet(int sockfd, struct addrinfo *ai);

#endif
