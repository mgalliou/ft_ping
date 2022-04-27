/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_sleep.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 13:45:17 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/27 14:35:27 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <signal.h>
#include <unistd.h>

static void	alrm_handler(int i)
{
	(void)i;
	g_p.alrm = 1;
}

void	ping_sleep(unsigned sec)
{
	g_p.alrm = 0;
	signal(SIGALRM, alrm_handler);
	alarm(sec);
	while (!g_p.alrm)
	{
	}
}
