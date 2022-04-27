/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_addrinfo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:32:01 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/27 14:50:03 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <stdio.h>
#include <unistd.h>

int	build_addrinfo(struct addrinfo **ai, char *host)
{
	struct addrinfo	hints;
	int				s;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if ((s = getaddrinfo(host, NULL, &hints, ai)))
	{
		fprintf(stderr, "ping: failed to getaddrinfo for %s\n", host);
		return (-1);
	}
	return (1);
}
