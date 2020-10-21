/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2020/10/21 12:30:16 by mgalliou         ###   ########.fr       */
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

void read_msghdr(struct msghdr *msghdr)
{
	struct cmsghdr  *cmsghdr;

	cmsghdr = CMSG_FIRSTHDR(msghdr);
}

int build_msghdr_and_recv(struct msghdr *msghdr, struct addrinfo **res)
{
	struct iovec    iov;
	char			packet[IP_MAXPACKET];
	int				ret;

	ft_bzero(msghdr, sizeof(*msghdr));
	msghdr->msg_name = res;
	msghdr->msg_namelen = sizeof(*res);
	iov.iov_base = packet;
	iov.iov_len = IP_MAXPACKET;
	msghdr->msg_iov = &iov;
	msghdr->msg_iovlen = 1;
	msghdr->msg_control = 0;
	msghdr->msg_controllen = 0;

	if (0 > (ret = recvmsg(penv.sockfd, msghdr, 0)))
	{
		fprintf(stderr, "recvmsg: %d\n", ret);
		perror(NULL);
		return (2);
	}
	return (0);
}

void build_icmphdr(struct icmp *icmphdr)
{
	ft_bzero(icmphdr, sizeof(icmphdr));
	icmphdr->icmp_type = ICMP_ECHO;
	icmphdr->icmp_code = 0;
	icmphdr->icmp_id = getpid();
	icmphdr->icmp_seq = 0;
	icmphdr->icmp_cksum = in_cksum((u_short*)icmphdr, sizeof(*icmphdr));
}

static int open_and_setup_socket(struct addrinfo *res)
{
	int             sockfd;

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	penv.sockfd = sockfd;
	if (0 > sockfd)
	{
		fprintf(stderr, "socket: %d\n", sockfd);
		perror(NULL);
		return (2);
	}
	penv.sokbuf_size = IP_MAXPACKET + 128;
	if (0 > (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF,
					(int*)&penv.sokbuf_size, sizeof(int))))
	{
		perror(NULL);
		return (2);
	}
	if (0 > (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,
					(int*)&penv.sokbuf_size, sizeof(int))))
	{
		perror(NULL);
		return (2);
	}
	return (0);
}

static int build_addrinfo(struct addrinfo **res)
{
	struct addrinfo hints;
	int             s;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	//hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_ICMP;

	if ((s = getaddrinfo(penv.host, NULL, &hints, res)))
	{
		fprintf(stderr, "getaddrinfo: %s: %s\n", penv.host, gai_strerror(s));
		return (2);
	}
	if (getuid())
	{
		(*res)->ai_socktype = SOCK_DGRAM;
	}
	/*	
	else
	{
		res->ai_socktype = SOCK_RAW;
	}
	*/
	return (0);
}

int main(int argc, char *argv[])
{
	struct addrinfo *res;
	int             ret;
	struct icmp     icmphdr;
	struct icmp     *reticmphdr;
	struct msghdr   msghdr;

	if (argc != 2)
	{
		fprintf(stderr, "usage: ping hostname\n");
	}
	penv.host = argv[1];
	build_addrinfo(&res);
	open_and_setup_socket(res);
	build_icmphdr(&icmphdr);
	if (0 > (ret = sendto(penv.sockfd, &icmphdr, sizeof(icmphdr),
					0, res->ai_addr, res->ai_addrlen)))
	{
		fprintf(stderr, "sendto: %d\n", ret);
		return (2);
	}
	build_msghdr_and_recv(&msghdr, &res);
	read_msghdr(&msghdr);
	//freeaddrinfo(res); // free the linked list
	//ft_putnbr(msghdr.msg_flags);
	printf("type: %d, code: %d, id: %d, seq: %d",
			reticmphdr->icmp_type,
			reticmphdr->icmp_code,
			reticmphdr->icmp_id,
			reticmphdr->icmp_seq);
	ft_putchar('\n');
	return (0);
}
