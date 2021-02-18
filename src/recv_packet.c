/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_packet.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:39:41 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/18 10:52:37 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

static int			print_packet(char *buf, int msglen)
{
	struct ip	*ip;
	struct icmp	*icmp;
	int			hlen;
	char		as[20];

	ip = (struct ip *)buf;
	hlen = ip->ip_hl << 2;
	if (msglen < hlen + ICMP_MINLEN) {
		return (0);
	}
	icmp = (struct icmp*)(buf + hlen);
	if (icmp->icmp_id != getpid())
	{
			return(0);
	}
	if (icmp->icmp_type != ICMP_ECHOREPLY) {
			//return(0);
	}
	inet_ntop(AF_INET, &ip->ip_src, as, 20);
	printf("%d bytes from %s: icmp_seq=%d ttl=%d",
			msglen,
			as,
			icmp->icmp_seq,
			ip->ip_ttl);
	/*
	/printf("type: %d, code: %d, id: %d, seq: %d\n",
			icmp->icmp_type,
			icmp->icmp_code,
			icmp->icmp_id,
			icmp->icmp_seq);
			*/
	return (1);
}

/*
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
*/

static void			prep_msghdr(struct msghdr *msghdr, struct addrinfo *ai)
{
	char			packet[IP_MAXPACKET];
	struct iovec	iov[1];
	//char 			ctrl[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(int))];

	ft_bzero(msghdr, sizeof(*msghdr));
	msghdr->msg_name = ai->ai_addr;
	msghdr->msg_namelen = ai->ai_addrlen;
	iov[0].iov_base = packet;
	iov[0].iov_len = IP_MAXPACKET;
	msghdr->msg_iov = iov;
	msghdr->msg_iovlen = 1;
	//msghdr->msg_control = 0;
	//msghdr->msg_controllen = 0;
}

int					recv_packet(int sockfd, struct addrinfo *ai)
{
	struct msghdr   msghdr;
	int				done;
	int				msglen;

	done = 0;
	while (!done)
	{
		prep_msghdr(&msghdr, ai);
		if (0 > (msglen = recvmsg(sockfd, &msghdr, 0)))
		{
			fprintf(stderr, "recvmsg: %d\n", msglen);
			return (msglen);
		}
//		read_msghdr(&msghdr);
		if (print_packet((msghdr.msg_iov[0]).iov_base, msglen))
		{
			done = 1;
		}
	}
	return (msglen);
}
