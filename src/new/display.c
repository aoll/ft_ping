/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 00:49:02 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 01:14:26 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	display(
	struct icmphdr *hdr2, struct timeval *t_time2, struct ip *iph2, t_env *e)
{
	struct timeval t_time;
	gettimeofday(&t_time, NULL);
	float t = ((1000000*t_time.tv_sec + t_time.tv_usec)
		- (1000000*(t_time2->tv_sec) + t_time2->tv_usec)) * 0.001;
	int it = t;
	if (t - it >= 0.1)
	{
		printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n",
		PACKETSIZE_TOTAL - sizeof(struct ip), e->ipv4, hdr2->un.echo.sequence,
		iph2->ip_ttl, t);
	}
	else
	{
		printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%d ms\n",
		PACKETSIZE_TOTAL - sizeof(struct ip), e->ipv4,
		hdr2->un.echo.sequence,  iph2->ip_ttl, it);
	}
	e->total += t ;
	e->min = e->min == 0 || t  < e->min ? t  : e->min;
	e->max = t  > e->max ? t  : e->max;
	return ;
}
