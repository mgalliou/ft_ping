/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_socket.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:41:00 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 15:33:47 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>
#include <netinet/ip.h>

int setup_socket(struct addrinfo *res)
{
	int             sockfd;
	int				ttl_val;
	struct timeval	tv_out; 

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	g_p.sockfd = sockfd;
	if (0 > sockfd)
	{
		fprintf(stderr, "Failed to open socket: %d\n", sockfd);
		return (-1);
	}
	ttl_val = 128;
	if (0 > setsockopt(sockfd, SOL_IP, IP_TTL, &ttl_val, sizeof(ttl_val))) 
	{ 
		fprintf(stderr, "Failed to set ttl sockopt.\n"); 
		return (-1); 
	} 
	tv_out.tv_sec = 1; 
	tv_out.tv_usec = 0;
	if (0 > setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out))
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
	return (0);
}


