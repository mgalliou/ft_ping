/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_packet.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 11:20:00 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/22 22:16:57 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <stdio.h>


int send_packet(int sockfd, struct icmp *icmp, struct addrinfo *ai)
{
	int             ret;

	if (0 > (ret = sendto(sockfd, icmp, sizeof(*icmp),
					0, ai->ai_addr, ai->ai_addrlen)))
	{
		fprintf(stderr, "sendto: %d\n", ret);
	}
	return (ret);
} 
