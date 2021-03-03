/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/03/03 13:28:27 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

struct ping g_p;

void print_ping_hdr(char *host, struct addrinfo *ai)
{
	char		as[20];

	inet_ntop(AF_INET, &((struct sockaddr_in*)(ai->ai_addr))->sin_addr, as, 20);
	printf("PING %s (%s) %d(%ld)\n",
			host,
			as,
			ICMP_DATALEN,
			ICMP_MINLEN + ICMP_DATALEN + sizeof(struct ip));
} 

int main(int argc, char *argv[])
{
	char	*host;
	struct	addrinfo *ai;
	int		sockfd;

	if (argc != 2)
	{
		fprintf(stderr, "usage: ping hostname\n");
		return (EXIT_FAILURE);
	}
	host = argv[1];
	g_p.host = host;
	if (0 != getuid())
	{
		fprintf(stderr, "ft_ping must be run as root");
		return (EXIT_FAILURE);
	}
	if (0 > build_addrinfo(&ai, host))
	{
		fprintf(stderr, "failed to build addrinfo");
		return (EXIT_FAILURE);
	}
	g_p.ai = ai;
	sockfd = setup_socket(ai);
	if (0 > sockfd)
	{
		fprintf(stderr, "failed to open socket");
		return (EXIT_FAILURE);
	}
	g_p.nsent = 0;
	g_p.nrcvd = 0;
	g_p.nerror = 0;
	print_ping_hdr(host, ai);
	ping_loop(sockfd, ai);
	return (EXIT_SUCCESS);
}
