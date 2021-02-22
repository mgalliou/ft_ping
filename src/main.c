/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/22 23:00:47 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>


int        	g_alrm_to = 0;

static void alrm_handler(int i)
{
	(void)i;
	g_alrm_to = 1;
}

void ft_sleep(unsigned sec)
{
	g_alrm_to = 0;
	signal(SIGALRM, alrm_handler);
	alarm(sec);
	while (!g_alrm_to)
	{
	}
}

static void build_icmp(struct icmp *icmp, int seq)
{
	ft_bzero(icmp, sizeof(*icmp));
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = getpid();
	icmp->icmp_seq = seq;
	icmp->icmp_cksum = in_cksum((u_short*)icmp, sizeof(*icmp));
}

static int 	ping_loop(int sockfd, struct addrinfo *ai)
{
	struct timeval	tvsend;
	struct timeval	tvrcv;
	long			timediff;
	struct icmp		icmp;
	static int		seq;

	seq = 1;
	while (1)
	{
		build_icmp(&icmp, seq);
		gettimeofday(&tvsend, NULL);
		if (0 > send_packet(sockfd, &icmp, ai))
		{
			fprintf(stderr, "failed to send packet\n");
			return (EXIT_FAILURE);
		}
		if (0 > recv_packet(sockfd, ai))
		{
			fprintf(stderr, "failed to receive packet\n");
		}
		else
		{
			gettimeofday(&tvrcv, NULL);
			timediff = tvrcv.tv_sec - tvsend.tv_sec;
			timediff = timediff * 1000000 + tvrcv.tv_usec - tvsend.tv_usec;
			printf(" time=%ld.%02ld ms\n", timediff / 1000, (timediff % 1000) / 10);
			ft_sleep(1);
		}
	}
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
	sockfd = setup_socket(ai);
	if (0 > sockfd)
	{
		fprintf(stderr, "failed to open socket");
		return (EXIT_FAILURE);
	}
	ping_loop(sockfd, ai);
	return (EXIT_SUCCESS);
}
