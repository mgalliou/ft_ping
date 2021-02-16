/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_socket.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:41:00 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 10:47:27 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>
#include <netinet/ip.h>

int setup_socket(struct addrinfo *res)
{
	int             sockfd;

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	g_p.sockfd = sockfd;
	if (0 > sockfd)
	{
		fprintf(stderr, "socket: %d\n", sockfd);
		perror(NULL);
		return (2);
	}
	g_p.sokbuf_size = IP_MAXPACKET + 128;
	if (0 > (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF,
					(int*)&g_p.sokbuf_size, sizeof(int))))
	{
		perror(NULL);
		return (2);
	}
	if (0 > (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,
					(int*)&g_p.sokbuf_size, sizeof(int))))
	{
		perror(NULL);
		return (2);
	}
	return (0);
}


