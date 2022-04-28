/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 13:14:53 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/27 18:01:28 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>

static void	prep_msghdr(struct msghdr *msghdr)
{
	char			packet[IP_MAXPACKET];
	struct iovec	iov[1];

	ft_bzero(msghdr, sizeof(*msghdr));
	ft_bzero(packet, sizeof(packet));
	iov[0].iov_base = packet;
	iov[0].iov_len = IP_MAXPACKET;
	msghdr->msg_iov = iov;
	msghdr->msg_iovlen = 1;
}

static void	int_handler(int i)
{
	struct timeval	end;
	long			diff;
	int				loss;

	(void)i;
	gettimeofday(&end, NULL);
	diff = tv_diff_in_ms(&g_p.start, &end);
	loss = 0;
	if (g_p.nsent > g_p.nrcvd)
	{
		loss = ((g_p.nsent - g_p.nrcvd) / (float)g_p.nsent) * 100;
	}
	printf("\n--- %s ping statistics ---\n", g_p.host);
	printf("%d packets transmitted, %d received",
		g_p.nsent, g_p.nrcvd);
	if (0 < g_p.nerror)
	{
		printf(", +%d errors", g_p.nerror);
	}
	printf(", %d%% packet loss, time %ldms\n", loss, diff);
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms",
		g_p.rtt_min, g_p.rtt_avg, g_p.rtt_max, g_p.rtt_mdev);
	freeaddrinfo(g_p.ai);
	exit(EXIT_FAILURE);
}

int	send_ping(int sockfd, struct addrinfo *ai)
{
	char			icmp[ICMP_MINLEN + ICMP_DATALEN];
	int				ret;

	build_icmp((struct icmp *)&icmp, sizeof(icmp));
	ret = sendto(sockfd, icmp, sizeof(icmp), 0, ai->ai_addr, ai->ai_addrlen);
	g_p.nsent++;
	return (ret);
}

void	recv_pong(int sockfd, struct msghdr *msghdr, int opt)
{
	struct ip		*pong;
	int				msglen;
	struct timeval	recvd;
	int				ret;

	ret = 0;
	while (!ret)
	{
		msglen = recvmsg(sockfd, msghdr, 0);
		gettimeofday(&recvd, NULL);
		if (0 < msglen)
		{
			pong = msghdr->msg_iov[0].iov_base;
		}
		if (msglen >= (int) sizeof(struct ip) + ICMP_MINLEN)
		{
			ret = print_packet(pong, msglen, &recvd, opt);
			if (ret)
			{
				ping_sleep(1);
			}
		}
	}
}

void	ping_loop(int sockfd, struct addrinfo *ai, int opt)
{
	struct msghdr	msghdr;

	gettimeofday(&g_p.start, NULL);
	signal(SIGINT, int_handler);
	while (1)
	{
		prep_msghdr(&msghdr);
		if (0 > send_ping(sockfd, ai))
		{
			fprintf(stderr, "failed to send packet\n");
		}
		else
		{
			recv_pong(sockfd, &msghdr, opt);
		}
	}
}
