/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tv_diff_in_ms.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgalliou <mgalliou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/03 13:40:44 by mgalliou          #+#    #+#             */
/*   Updated: 2022/04/27 14:53:25 by mgalliou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>

static long	tvtoms(struct timeval *tv)
{
	long	ms;

	ms = tv->tv_sec * 1000;
	ms += tv->tv_usec / 1000;
	return (ms);
}

long	tv_diff_in_ms(struct timeval *beg, struct timeval *end)
{
	return (tvtoms(end) - tvtoms(beg));
}
