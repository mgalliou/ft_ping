/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_packet.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 11:29:40 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/24 12:31:07 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>

static long			print_time(struct timeval *send)
{
	struct timeval	now;
	long			diff;

	gettimeofday(&now, NULL);
	diff = now.tv_sec - send->tv_sec;
	diff = diff * 1000000 + now.tv_usec - send->tv_usec;
	printf(" time=%ld.%02ld ms", diff / 1000, (diff % 1000) / 10);
	return (diff);
}

int			print_packet(char *buf, int msglen)
{
	struct ip	*ip;
	struct icmp	*icmp;
	int			hlen;
	char		as[20];

	ip = (struct ip *)buf;
	hlen = sizeof(struct ip);
	if (msglen < hlen + ICMP_MINLEN) {
		return (0);
	}
	icmp = (struct icmp*)(buf + hlen);
	if (icmp->icmp_id != getpid())
	{
		return (0);
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
			print_time((struct timeval*)&icmp->icmp_dun.id_ts);
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
