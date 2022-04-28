/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_addrinfo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 12:32:01 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/28 12:38:09 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <stdio.h>
#include <unistd.h>

int	build_addrinfo(struct addrinfo **ai, char *host)
{
	struct addrinfo	hints;

	ft_bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	return (getaddrinfo(host, NULL, &hints, ai));
}
