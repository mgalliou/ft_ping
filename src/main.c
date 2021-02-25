/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/25 16:53:22 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <arpa/inet.h>

int        	g_alrm_to = 0;
struct ping g_p;

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

static void build_icmp(struct icmp *icmp, int len)
{
	static int seq = 1;

	ft_bzero(icmp, len);
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = getpid();
	icmp->icmp_seq = seq;
	if (ICMP_DATALEN >= sizeof(struct timeval))
	{
		gettimeofday((struct timeval*)&icmp->icmp_dun.id_ts, NULL);
	}
	icmp->icmp_cksum = in_cksum((u_short*)icmp, len);
	seq++;
}

static int	ft_tvtoms(struct timeval *tv)
{
	long	ms;

	ms = tv->tv_sec * 1000;
	ms += tv->tv_usec / 1000;
	return ms;
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
		loss = (g_p.nsent - g_p.nrcvd) / g_p.nsent;
	}
	printf("\n--- %s ping statistics ---\n", g_p.host);
	printf("%d packets transmited, %d received, %d%% packet lost, time %ldms\n",
			g_p.nsent, g_p.nrcvd, loss, diff);
	exit(EXIT_FAILURE);
}

static int 	ping_loop(int sockfd, struct addrinfo *ai)
{
	char			icmp[ICMP_MINLEN + ICMP_DATALEN];
	struct msghdr   msghdr;
	int				msglen;
	int				done;

	signal(SIGINT, int_handler);
	gettimeofday(&g_p.start, NULL);
	while (1)
	{
		build_icmp((struct icmp*)&icmp, sizeof(icmp));
		if (0 > sendto(sockfd, icmp, sizeof(icmp),
					0, ai->ai_addr, ai->ai_addrlen))
		{
			fprintf(stderr, "failed to send packet\n");
			return (EXIT_FAILURE);
		}
		g_p.nsent++;
		done = 0;
		while (!done)
		{
			prep_msghdr(&msghdr);
			msglen = recv_packet(sockfd, &msghdr);
			if (0 > msglen)
			{
				fprintf(stderr, "failed to receive packet\n");
			}
			else
			{
				g_p.nrcvd++;
				if (print_packet((msghdr.msg_iov[0]).iov_base, msglen))
				{
					done = 1;
				} 
			}
		} 
		ft_sleep(1);
	}
}

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
	sockfd = setup_socket(ai);
	if (0 > sockfd)
	{
		fprintf(stderr, "failed to open socket");
		return (EXIT_FAILURE);
	}
	g_p.nsent = 0;
	g_p.nrcvd = 0;
	print_ping_hdr(host, ai);
	ping_loop(sockfd, ai);
	return (EXIT_SUCCESS);
}
