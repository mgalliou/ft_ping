/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 15:56:14 by mgalliou         ###   ########.fr       */
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
	char	*host;

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
	if (0 > setup_socket(ai))
	{
		fprintf(stderr, "failed to open socket");
		return (EXIT_FAILURE);
	} 
	if (0 > send_packet(ai))
	{
		fprintf(stderr, "failed to send packet");
		return (EXIT_FAILURE);
	} 
	if (0 > recv_packet(ai))
	{
		fprintf(stderr, "failed to receive packet");
		return (EXIT_FAILURE);
	} 
	return (EXIT_SUCCESS);
}
