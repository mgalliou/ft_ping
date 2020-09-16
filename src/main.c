/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2020/09/16 10:36:42 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include "ft_ping.h"

int main(int argc, char *argv[])
{
	struct addrinfo hints;
	struct addrinfo *res;
	int             sockfd;
	int             s;
	int             ret;
	struct icmp     icmphdr;
	struct icmp     *reticmphdr;
	struct msghdr   msghdr;
	struct iovec    iov;
	char packet[IP_MAXPACKET];

	if (argc != 2)
	{
		fprintf(stderr, "usage: ping hostname\n");
	}
	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	//hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_ICMP;
	if ((s = getaddrinfo(argv[1], NULL, &hints, &res)))
	{
		fprintf(stderr, "getaddrinfo: %s: %s\n", argv[1], gai_strerror(s));
		return (2);
	}

	if (getuid())
	{
		res->ai_socktype = SOCK_DGRAM;
	}
	if (0 > (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)))
	{
		fprintf(stderr, "socket: %d\n", sockfd);
		perror(NULL);
		return (2);
	}
	penv.sokbuf_size = IP_MAXPACKET + 128;
	if (0 > (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (int*)&penv.sokbuf_size, sizeof(int))))
	{
		perror(NULL);
		return (2);
	}
	if (0 > (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (int*)&penv.sokbuf_size, sizeof(int))))
	{
		perror(NULL);
		return (2);
	}

	ft_bzero(&icmphdr, sizeof(icmphdr));
	icmphdr.icmp_type = ICMP_ECHO;
	icmphdr.icmp_code = 0;
	icmphdr.icmp_id = getpid();
	icmphdr.icmp_seq = 0;
	icmphdr.icmp_cksum = in_cksum((u_short*)&icmphdr, sizeof(icmphdr));
	if (0 > (ret = sendto(sockfd, &icmphdr, sizeof(icmphdr), 0, res->ai_addr, res->ai_addrlen)))
	{
		fprintf(stderr, "sendto: %d\n", ret);
		return (2);
	}
	ft_bzero(&msghdr, sizeof(msghdr));
	msghdr.msg_name = &res;
	msghdr.msg_namelen = sizeof(res);
	iov.iov_base = packet;
	iov.iov_len = IP_MAXPACKET;
	msghdr.msg_iov = &iov;
	msghdr.msg_iovlen = 1;
	msghdr.msg_control = 0;
	msghdr.msg_controllen = 0;
	if (0 > (ret = recvmsg(sockfd, &msghdr, 0)))
	{
		fprintf(stderr, "recvmsg: %d\n", ret);
		perror(NULL);
		return (2);
	}
	//freeaddrinfo(res); // free the linked list
	//ft_putnbr(msghdr.msg_flags);
	reticmphdr = msghdr.msg_iov->iov_base;
	ft_putnbr(reticmphdr->icmp_type);
	ft_putchar('\n');
	return (0);
}
