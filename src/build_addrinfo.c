/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_addrinfo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:32:01 by mgalliou          #+#    #+#             */
/*   Updated: 2021/02/25 11:49:06 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <stdio.h>
#include <unistd.h>

int build_addrinfo(struct addrinfo **ai, char *host)
{
	struct addrinfo hints;
	int             s;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	//hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_ICMP;
	*ai = malloc(sizeof(*ai));
	if ((s = getaddrinfo(host, NULL, &hints, ai)))
	{
		fprintf(stderr, "getaddrinfo: %s: %s\n", host, gai_strerror(s));
		return (-1);
	}
	return (1);
}
