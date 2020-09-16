/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   in_cksum.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 10:00:08 by mgalliou          #+#    #+#             */
/*   Updated: 2020/09/16 10:19:47 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>

/*
** https://opensource.apple.com/source/network_cmds/network_cmds-433/ping.tproj/ping.c.auto.html   
*/

u_short in_cksum(u_short *addr, int len)
{
	int nleft, sum;
	u_short *w;
	union {
		u_short	us;
		u_char	uc[2];
	} last;
	u_short answer;

	nleft = len;
	sum = 0;
	w = addr;

	/*
	 * Our algorithm is simple, using a 32 bit accumulator (sum), we add
	 * sequential 16 bit words to it, and at the end, fold back all the
	 * carry bits from the top 16 bits into the lower 16 bits.
	 */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1) {
		last.uc[0] = *(u_char *)w;
		last.uc[1] = 0;
		sum += last.us;
	}

	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return(answer);
}

/*
**void compute_checksum(struct icmp *hdr)
**{
**	int16_t *ptr;
**	int16_t cksum;
**	int16_t last;
**	int     len;
**
**	ptr = (int16_t*)hdr;
**	cksum = 0;
**	len = sizeof(hdr);
**	while (len)
**	{
**		cksum += *ptr;
**		ptr++;
**		len -= 2;
**		if (len == 1)
**		{
**			last = (char)*ptr;
**			last <<= 8;
**			cksum += last;
**			len = 0;
**		}
**	}
**	hdr->icmp_cksum = ~cksum;
**}
*/
