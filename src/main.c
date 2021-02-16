/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 11:33:11 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>

t_ping_env g_p;

static int print_packet(char *buf, int ret)
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

static void read_msghdr(struct msghdr *msghdr)
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

static void			build_msghdr(struct msghdr *msghdr, struct addrinfo **res)
{
	struct iovec	iov;
	//char 			ctrl[CMSG_SPACE(sizeof(struct timeval)) + CMSG_SPACE(sizeof(int))];

	ft_bzero(msghdr, sizeof(*msghdr));
	msghdr->msg_name = res;
	msghdr->msg_namelen = sizeof(*res);
	iov.iov_base = g_p.packet;
	iov.iov_len = IP_MAXPACKET;
	msghdr->msg_iov = &iov;
	msghdr->msg_iovlen = 1;
	msghdr->msg_control = 0;
	msghdr->msg_controllen = 0;
}


int main(int argc, char *argv[])
{
	struct addrinfo *ai;
	int             ret;
	//struct icmp	*reticmphdr;
	struct msghdr   msghdr;
	int				done;


	if (argc != 2)
	{
		fprintf(stderr, "usage: ping hostname\n");
	}
	g_p.host = argv[1];
	build_addrinfo(&ai);
	setup_socket(ai);
	send_packet(ai);
	done = 0;
	while (!done)
	{
		build_msghdr(&msghdr, &ai);
		if (0 > (ret = recvmsg(g_p.sockfd, &msghdr, MSG_WAITALL)))
		{
			fprintf(stderr, "recvmsg: %d\n", ret);
			perror(NULL);
			return (2);
		}
		read_msghdr(&msghdr);
		if (print_packet(g_p.packet, ret))
		{
			done = 1;
		}
	} 
	return (0);
}
