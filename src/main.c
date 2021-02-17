/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 15:33:34 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/17 10:00:52 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <signal.h>

t_ping_env g_p;
int        g_alrm_to = 0;

static void alrm_handler(int i)
{
	(void) i;
	g_alrm_to = 1;
}

void ft_sleep(unsigned sec)
{
	g_alrm_to = 0;
	signal(SIGALRM, alrm_handler);
	alarm(sec);
	while (!g_alrm_to)
	{
	}
}

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
	while (1)
	{
		if (0 > send_packet(ai))
		{
			fprintf(stderr, "failed to send packet\n");
			return (EXIT_FAILURE);
		}
		if (0 > recv_packet(ai))
		{
			fprintf(stderr, "failed to receive packet\n");
		}
		ft_sleep(1);
	}
	return (EXIT_SUCCESS);
}
