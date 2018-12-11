/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 00:49:02 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 03:34:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void	display(
	struct icmphdr *hdr2, struct timeval *t_time2, struct ip *iph2, t_env *e)
{
	struct timeval t_time;
	gettimeofday(&t_time, NULL);
	int time_transfert = ((1000000*t_time.tv_sec + t_time.tv_usec)
		- (1000000*(t_time2->tv_sec) + t_time2->tv_usec))/ 100 ;
		// - (1000000*(t_time2->tv_sec) + t_time2->tv_usec)) / 100;
	float time_transfert_u = ((1000000*t_time.tv_sec + t_time.tv_usec)
		- (1000000*(t_time2->tv_sec) + t_time2->tv_usec));

	float tf = time_transfert * 0.1;
	int t = time_transfert / 10;
	float f = t * 1.0;
	// int it = t;
	// printf("f t: %f, i it: %d\n", t, it );
	if (f != tf)
	// if (t - it >= 0.1)
	{
		printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%0.1f ms\n",
		PACKETSIZE_TOTAL - sizeof(struct ip), e->ipv4, hdr2->un.echo.sequence,
		iph2->ip_ttl, tf);
		// iph2->ip_ttl, t);
	}
	else
	{
		printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%d ms\n",
		PACKETSIZE_TOTAL - sizeof(struct ip), e->ipv4,
		hdr2->un.echo.sequence,  iph2->ip_ttl, t);
		// hdr2->un.echo.sequence,  iph2->ip_ttl, it);
	}
	printf("time_transfert: %d\n", time_transfert);
	printf("time_transfert_u: %f\n", time_transfert_u);
	time_transfert_u /= 1000;
	e->total += time_transfert_u;
	e->min = e->min == 0 || time_transfert_u  < e->min ? time_transfert_u  : e->min;
	e->max = time_transfert_u  > e->max ? time_transfert_u  : e->max;
	return ;
}
