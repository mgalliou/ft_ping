/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_icmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 17:05:23 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/28 14:25:13 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <libft.h>
#include <unistd.h>
#include <sys/time.h>

void	build_icmp(struct icmp *icmp, int len)
{
	static int	seq = 1;

	ft_bzero(icmp, len);
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = getpid();
	icmp->icmp_seq = seq;
	seq++;
	if (ICMP_DATALEN >= sizeof(struct timeval))
	{
		gettimeofday((struct timeval *)&icmp->icmp_data, NULL);
	}
	icmp->icmp_cksum = in_cksum((u_short *)icmp, len);
}
