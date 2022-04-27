/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_packet.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 11:29:40 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/27 14:56:10 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>

static float	ping_abs(float n)
{
	if (n < 0)
	{
		n = n * -1;
	}
	return (n);
}

static void	print_time(struct timeval *sent, struct timeval *recvd)
{
	float			diff;

	diff = recvd->tv_sec - sent->tv_sec;
	diff = diff * 1000 + recvd->tv_usec - sent->tv_usec;
	diff /= 1000;
	if (diff < g_p.rtt_min || !g_p.rtt_min)
	{
		g_p.rtt_min = diff;
	}
	if (g_p.rtt_avg)
	{
		if (g_p.rtt_mdev)
		{
			g_p.rtt_mdev = (g_p.rtt_mdev + ping_abs(diff - g_p.rtt_avg)) / 2;
		}
		else
		{
			g_p.rtt_mdev = ping_abs(diff - g_p.rtt_avg);
		}
	}
	if (g_p.rtt_avg)
	{
		g_p.rtt_avg = (g_p.rtt_avg + diff) / 2;
	}
	else
	{
		g_p.rtt_avg = diff;
	}
	if (diff > g_p.rtt_max)
	{
		g_p.rtt_max = diff;
	}
	printf(" time=%.2f ms", diff);
}

static const char	*get_icmp_desc(int type)
{
	static const char	*desc[NR_ICMP_TYPES];
	static int			i = 0;

	if (!i)
	{
		desc[ICMP_ECHOREPLY] = "Echo Reply";
		desc[ICMP_DEST_UNREACH] = "Destination Unreachable";
		desc[ICMP_SOURCE_QUENCH] = "Source Quench";
		desc[ICMP_REDIRECT] = "Redirect (change route)";
		desc[ICMP_ECHO] = "Echo Request";
		desc[ICMP_TIME_EXCEEDED] = "Time Exceeded";
		desc[ICMP_PARAMETERPROB] = "Parameter Problem";
		desc[ICMP_TIMESTAMP] = "Timestamp Request";
		desc[ICMP_TIMESTAMPREPLY] = "Timestamp Reply";
		desc[ICMP_INFO_REQUEST] = "Information Request";
		desc[ICMP_INFO_REPLY] = "Information Reply";
		desc[ICMP_ADDRESS] = "Address Mask Request";
		desc[ICMP_ADDRESSREPLY] = "Address Mask Reply";
		i = 1;
	}
	return (desc[type]);
}

int	print_packet(struct ip *ip, int msglen, struct timeval *recvd, int opt)
{
	struct icmp	*icmp;
	char		as[20];

	icmp = (struct icmp *)((unsigned char *)ip + sizeof(struct ip));
	inet_ntop(AF_INET, &ip->ip_src, as, 20);
	if (icmp->icmp_type == ICMP_ECHOREPLY)
	{
		if (icmp->icmp_id != getpid())
		{
			return (0);
		}
		printf("%ld bytes from %s: icmp_seq=%d",
			msglen - sizeof(struct ip), as, icmp->icmp_seq);
		printf(" ttl=%d", ip->ip_ttl);
		if (msglen - sizeof(struct ip) - sizeof(struct timeval) >= ICMP_MINLEN)
		{
			print_time((struct timeval *)&icmp->icmp_data, recvd);
			printf("\n");
		}
		g_p.nrcvd++;
	}
	else
	{
		if (opt & O_VERBOSE)
		{
			printf("From %s: icmp_seq=%d", as, icmp->icmp_seq);
			printf(" %s\n", get_icmp_desc(icmp->icmp_type));
		}
		g_p.nerror++;
	}
	return (1);
}
