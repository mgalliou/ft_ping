/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/04 14:34:57 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/29 12:44:14 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <netdb.h>
# include <netinet/ip_icmp.h>

# define ICMP_DATALEN	56
# define O_VERBOSE		1

extern struct s_ping	g_p;

struct					s_ping
{
	char				*host;
	struct addrinfo		*ai;
	//int				intrv;
	int					nsent;
	int					nrcvd;
	int					nerror;
	struct timeval		start;
	int					alrm;
	float				rtt_min;
	float				rtt_avg;
	float				rtt_max;
	float				rtt_mdev;
};

int				build_addrinfo(struct addrinfo **ai, char *host);
int				setup_socket(struct addrinfo *ai);
void			build_icmp(struct icmp *icmp, int len);
void			ping_loop(int sockfd, struct addrinfo *ai, int opt);
void			ping_sleep(unsigned sec);
int				print_packet(struct ip *ip, int msglen, struct timeval *recvd, int opt);

unsigned short	compute_checksum(unsigned short *hdr, int len);
long			tv_diff_in_ms(struct timeval *beg, struct timeval *end);

#endif
