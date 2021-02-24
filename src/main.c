/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/24 09:39:05 by mgalliou         ###   ########.fr       */
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
#include <arpa/inet.h>

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

static long			get_time_diff(struct timeval *send)
{
	struct timeval	now;
	long			diff;

	gettimeofday(&now, NULL);
	diff = now.tv_sec - send->tv_sec;
	diff = diff * 1000000 + now.tv_usec - send->tv_usec;
	return (diff);
}

static int			print_packet(char *buf, int msglen, struct timeval send)
{
	struct ip	*ip;
	struct icmp	*icmp;
	int			hlen;
	char		as[20];
	long		timediff;
	(void)(send);

	ip = (struct ip *)buf;
	hlen = sizeof(struct ip);
	if (msglen < hlen + ICMP_MINLEN) {
		return (0);
	}
	icmp = (struct icmp*)(buf + hlen);
	if (icmp->icmp_id != getpid())
	{
		printf("bad pid! ");
	}
	inet_ntop(AF_INET, &ip->ip_src, as, 20);
	printf("%d bytes from %s: icmp_seq=%d", msglen - hlen, as, icmp->icmp_seq);
	//printf(" type=%d", icmp->icmp_type);
	if (icmp->icmp_type == ICMP_TIME_EXCEEDED) {
		printf("Time to live exceeded");
		return (0);
	}
	else
	{
		printf(" ttl=%d", ip->ip_ttl);
		if (ICMP_DATALEN >= sizeof(struct timeval))
		{
			timediff = get_time_diff((struct timeval*)&icmp->icmp_dun.id_ts);
			printf(" time=%ld.%02ld ms", timediff / 1000, (timediff % 1000) / 10);
		}
	}
	printf("\n");
	/*
	printf("type: %d, code: %d, id: %d, seq: %d\n",
			icmp->icmp_type,
			icmp->icmp_code,
			icmp->icmp_id,
			icmp->icmp_seq);
			*/
	return (1);
}


static void			prep_msghdr(struct msghdr *msghdr, struct addrinfo *ai)
{
	char			packet[IP_MAXPACKET];
	struct iovec	iov[1];
	//char 			ctrl[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(int))];

	(void)(ai);
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

static int 	ping_loop(int sockfd, struct addrinfo *ai)
{
	struct timeval	tvsend;
	char			icmp[ICMP_MINLEN + ICMP_DATALEN];
	struct msghdr   msghdr;
	int				msglen;

	while (1)
	{
		build_icmp((struct icmp*)&icmp, sizeof(icmp));
		if (0 > sendto(sockfd, icmp, sizeof(icmp),
					0, ai->ai_addr, ai->ai_addrlen))
		{
			fprintf(stderr, "failed to send packet\n");
			return (EXIT_FAILURE);
		}
		prep_msghdr(&msghdr, ai);
		msglen = recv_packet(sockfd, &msghdr);
		if (0 > msglen)
		{
			fprintf(stderr, "failed to receive packet\n");
		}
		else
		{
			print_packet((msghdr.msg_iov[0]).iov_base, msglen, tvsend);
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
	print_ping_hdr(host, ai);
	ping_loop(sockfd, ai);
	return (EXIT_SUCCESS);
}
