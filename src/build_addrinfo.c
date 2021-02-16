/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_addrinfo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:32:01 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/16 10:47:43 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <stdio.h>
#include <unistd.h>

int build_addrinfo(struct addrinfo **res)
{
	struct addrinfo hints;
	int             s;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	//hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_ICMP;

	if ((s = getaddrinfo(g_p.host, NULL, &hints, res)))
	{
		fprintf(stderr, "getaddrinfo: %s: %s\n", g_p.host, gai_strerror(s));
		return (2);
	}
	if (getuid())
	{
		(*res)->ai_socktype = SOCK_DGRAM;
	}
	/*	
	else
	{
		res->ai_socktype = SOCK_RAW;
	}
	*/
	return (0);
}
