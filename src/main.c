/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2020/02/28 12:46:44 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <netdb.h>
#include <libft.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

void compute_checksum(struct icmp *hdr)
{
	(void)hdr;
}

int main(int argc, char *argv[])
{
	struct addrinfo hints;
	struct addrinfo *res;
	int             sockfd;
	int             s;
	int             ret;
	struct icmp     icmphdr;
	struct msghdr   msghdr;

	if (argc != 2)
	{
		fprintf(stderr, "usage: ping hostname\n");
	}

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;

	if ((s = getaddrinfo(argv[1], NULL, &hints, &res)))
	{
		fprintf(stderr, "getaddrinfo: bad hostname\n");
		return 2;
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	ft_bzero(&icmphdr, sizeof(icmphdr));
	icmphdr.icmp_type = ICMP_ECHO;
	icmphdr.icmp_code = 0;
	compute_checksum(&icmphdr);

	if (1 > (ret = sendto(sockfd, &icmphdr, sizeof(icmphdr), 0, (struct sockaddr*)res, sizeof(res))))
	{
		fprintf(stderr, "sendto: %d\n", ret);
		return 2;
	}
	ft_bzero(&msghdr, sizeof(msghdr));
	if (1 > (ret = recvmsg(sockfd, &msghdr, sizeof(icmphdr))))
	{
		fprintf(stderr, "recvmsg: %d\n", ret);
		return 2;
	}
	freeaddrinfo(res); // free the linked list
	ft_putnbr(msghdr.msg_flags);
	return 0;
}
