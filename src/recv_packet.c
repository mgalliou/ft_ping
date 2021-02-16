/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_packet.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:39:41 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 15:51:23 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>

static int			print_packet(char *buf, int ret)
{
	struct ip	*ip;
	struct icmp	*icmp;
	int			hlen;

	ip = (struct ip *)buf;
	hlen = ip->ip_hl << 2;
	if (ret < hlen + ICMP_MINLEN) {
		return (0);
	}
	icmp = (struct icmp*)(buf + hlen);
	if (icmp->icmp_type != ICMP_ECHOREPLY) {
			return(0);
	}
	if (icmp->icmp_id != getpid())
	{
			return(0);
	}
	printf("type: %d, code: %d, id: %d, seq: %d",
			icmp->icmp_type,
			icmp->icmp_code,
			icmp->icmp_id,
			icmp->icmp_seq);
	return (1);
}

static void 		read_msghdr(struct msghdr *msghdr)
{
	struct cmsghdr  *cmsghdr;
	struct timeval  *tv = NULL;

	cmsghdr = CMSG_FIRSTHDR(msghdr);
	while (cmsghdr)
	{
		if (cmsghdr->cmsg_level == SOL_SOCKET &&
				cmsghdr->cmsg_type == CMSG_LEN(sizeof *tv))
		{
			//memcpy(&now, CMSG_DATA(cmsghdr), sizeof(now));
		}
		cmsghdr = CMSG_NXTHDR(msghdr, cmsghdr);
	}
}

static void			prep_msghdr(struct msghdr *msghdr, struct addrinfo *ai)
{
	struct iovec	iov[1];
	//char 			ctrl[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(int))];

	ft_bzero(msghdr, sizeof(*msghdr));
	msghdr->msg_name = ai->ai_addr;
	msghdr->msg_namelen = ai->ai_addrlen;
	iov[0].iov_base = g_p.packet;
	iov[0].iov_len = IP_MAXPACKET;
	msghdr->msg_iov = iov;
	msghdr->msg_iovlen = 1;
	//msghdr->msg_control = 0;
	//msghdr->msg_controllen = 0;
}

int					recv_packet(struct addrinfo *ai)
{
	struct msghdr   msghdr;
	int				done;
	int				ret;

	done = 0;
	while (!done)
	{
		prep_msghdr(&msghdr, ai);
		if (0 > (ret = recvmsg(g_p.sockfd, &msghdr, 0)))
		{
			fprintf(stderr, "recvmsg: %d\n", ret);
			return (ret);
		}
		read_msghdr(&msghdr);
		if (print_packet(g_p.packet, ret))
		{
			done = 1;
		}
	}
	return (ret);
}
