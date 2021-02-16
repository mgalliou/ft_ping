/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_packet.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:20:00 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 11:32:33 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>

static void build_icmphdr(struct icmp *icmphdr)
{
	ft_bzero(icmphdr, sizeof(icmphdr));
	icmphdr->icmp_type = ICMP_ECHO;
	icmphdr->icmp_code = 0;
	icmphdr->icmp_id = getpid();
	icmphdr->icmp_seq = 0;
	icmphdr->icmp_cksum = in_cksum((u_short*)icmphdr, sizeof(*icmphdr));
}

int send_packet(struct addrinfo *ai)
{
	int             ret;
	struct icmp     icmphdr;

	build_icmphdr(&icmphdr);
	if (0 > (ret = sendto(g_p.sockfd, &icmphdr, sizeof(icmphdr),
					0, ai->ai_addr, ai->ai_addrlen)))
	{
		fprintf(stderr, "sendto: %d\n", ret);
	}
	return (ret);
} 
