/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_cksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 10:00:08 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/29 12:43:31 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

/*
** https://datatracker.ietf.org/doc/html/rfc1071
*/

unsigned short	compute_checksum(unsigned short *hdr, int len)
{
	unsigned short *ptr;
	int cksum;

	ptr = hdr;
	cksum = 0;
	while (len > 1)
	{
		cksum += *ptr++;
		len -= 2;
	}
	if (len > 0)
	{
		   cksum += (unsigned char )*ptr;
	}
	while (cksum >> 16)
	{
		cksum = (cksum & 0xffff) + (cksum >> 16);
	}
	return(~cksum);
}


