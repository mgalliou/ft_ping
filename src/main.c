/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/27 14:45:31 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

struct ping	g_p;

static void	print_ping_hdr(char *host, struct addrinfo *ai)
{
	char	as[20];

	inet_ntop(AF_INET,
		&((struct sockaddr_in *)(ai->ai_addr))->sin_addr,
		as,
		20);
	printf("PING %s (%s) %d(%ld)\n",
		host,
		as,
		ICMP_DATALEN,
		ICMP_MINLEN + ICMP_DATALEN + sizeof(struct ip));
}

static void	print_help(void)
{
	printf("\nUsage:\n  ping [options] <destination>\n");
}

static int	check_args(int argc, char *argv[], int *opt, char **host)
{
	int		i;

	*opt = 0;
	*host = NULL;
	i = 1;
	while (i < argc)
	{
		if (ft_strequ("-v", argv[i]))
		{
			*opt = *opt | O_VERBOSE;
		}
		else if (ft_strequ("-h", argv[i]))
		{
			print_help();
			exit(EXIT_SUCCESS);
		}
		else
		{
			*host = argv[i];
		}
		++i;
	}
	if (!*host)
	{
		fprintf(stderr, "usage: ping hostname\n");
		return (-1);
	}
	return (1);
}

int	main(int argc, char *argv[])
{
	char			*host;
	struct addrinfo	*ai;
	int				sockfd;
	int				opt;

	if (-1 == check_args(argc, argv, &opt, &host))
	{
		return (EXIT_FAILURE);
	}
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
	g_p.rtt_min = 0;
	g_p.rtt_avg = 0;
	g_p.rtt_max = 0;
	g_p.rtt_mdev = 0;
	print_ping_hdr(host, ai);
	ping_loop(sockfd, ai, opt);
	return (EXIT_SUCCESS);
}
