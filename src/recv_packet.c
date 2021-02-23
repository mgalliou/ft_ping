/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_packet.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:39:41 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/23 12:55:26 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>

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

int					recv_packet(int sockfd, struct msghdr *msghdr)
{
	int				msglen;

	if (0 > (msglen = recvmsg(sockfd, msghdr, 0)))
	{
		fprintf(stderr, "recvmsg: %d\n", msglen);
	}
	return (msglen);
}
