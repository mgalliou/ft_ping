/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_socket.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:41:00 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/27 14:52:43 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>
#include <netinet/ip.h>

int	setup_socket(struct addrinfo *res)
{
	int				sockfd;
	int				ttl;
	struct timeval	tv_out;

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (0 > sockfd)
	{
		fprintf(stderr, "Failed to open socket: %d\n", sockfd);
		return (-1);
	}
	ttl = IPDEFTTL;
	if (0 > setsockopt(sockfd, SOL_IP, IP_TTL, &ttl, sizeof(ttl)))
	{
		fprintf(stderr, "Failed to set ttl sockopt.\n");
		return (-1);
	}
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;
	if (0 > setsockopt(sockfd,
			SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)))
	{
		fprintf(stderr, "Failed to set recvtimeout sockopt.\n");
		return (-1);
	}
	/*
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
	*/
	return (sockfd);
}
