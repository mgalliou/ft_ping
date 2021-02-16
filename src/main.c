/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 11:49:00 by mgalliou         ###   ########.fr       */
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

int main(int argc, char *argv[])
{
	struct addrinfo *ai;
	//struct icmp	*reticmphdr;

	if (argc != 2)
	{
		fprintf(stderr, "usage: ping hostname\n");
	}
	g_p.host = argv[1];
	build_addrinfo(&ai);
	setup_socket(ai);
	send_packet(ai);
	recv_packet(&ai);
	return (0);
}
