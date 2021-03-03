/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 13:14:53 by mgalliou          #+#    #+#             */
/*   Updated: 2021/03/03 13:18:14 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>

static void alrm_handler(int i)
{
	(void)i;
	g_p.alrm = 1;
}

void ft_sleep(unsigned sec)
{
	g_p.alrm = 0;
	signal(SIGALRM, alrm_handler);
	alarm(sec);
	while (!g_p.alrm)
	{
	}
}

static int	ft_tvtoms(struct timeval *tv)
{
	long	ms;

	ms = tv->tv_sec * 1000;
	ms += tv->tv_usec / 1000;
	return ms;
}

static void			prep_msghdr(struct msghdr *msghdr)
{
	char			packet[IP_MAXPACKET];
	struct iovec	iov[1];
	//char 			ctrl[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(int))];

	ft_bzero(msghdr, sizeof(*msghdr));
	ft_bzero(packet, sizeof(packet));
	//msghdr->msg_name = ai->ai_addr;
	//msghdr->msg_namelen = ai->ai_addrlen;
	iov[0].iov_base = packet;
	iov[0].iov_len = IP_MAXPACKET;
	msghdr->msg_iov = iov;
	msghdr->msg_iovlen = 1;
	//msghdr->msg_control = 0;
	//msghdr->msg_controllen = 0;
}

static void int_handler(int i)
{
	(void)i;
	struct timeval	end;
	long			diff;
	int				loss;

	gettimeofday(&end, NULL);
	diff = ft_tvtoms(&end) - ft_tvtoms(&g_p.start);
	loss = 0;
	if (g_p.nsent > g_p.nrcvd)
	{
		loss = ((g_p.nsent - g_p.nrcvd) / g_p.nsent) * 100;
	}
	printf("\n--- %s ping statistics ---\n", g_p.host);
	printf("%d packets transmitted, %d received",
			g_p.nsent, g_p.nrcvd);
	if (0 < g_p.nerror)
	{
		printf(", +%d errors", g_p.nerror);
	}
	printf(", %d%% packet loss, time %ldms\n", loss, diff);
	freeaddrinfo(g_p.ai);
	exit(EXIT_FAILURE);
}

void			ping_loop(int sockfd, struct addrinfo *ai)
{
	char			icmp[ICMP_MINLEN + ICMP_DATALEN];
	struct msghdr   msghdr;
	int				msglen;

	gettimeofday(&g_p.start, NULL);
	signal(SIGINT, int_handler);
	while (1)
	{
		build_icmp((struct icmp*)&icmp, sizeof(icmp));
		if (0 > sendto(sockfd, icmp, sizeof(icmp),
					0, ai->ai_addr, ai->ai_addrlen))
		{
			fprintf(stderr, "failed to send packet\n");
		}
		else
		{
			g_p.nsent++;
			prep_msghdr(&msghdr);
			msglen = recvmsg(sockfd, &msghdr, 0);
			if (0 < msglen)
			{
				print_packet((msghdr.msg_iov[0]).iov_base, msglen);
				ft_sleep(1);
			}
		}
	}
}
