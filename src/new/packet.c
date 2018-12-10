/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 23:21:08 by alex              #+#    #+#             */
/*   Updated: 2018/12/10 01:23:48 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static unsigned short			checksum(unsigned short *buffer, int size)
{
	unsigned int cksum = 0;

	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(unsigned short);
	}
	if (size)
		cksum += *(unsigned char *)buffer;
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >>16);
	return (unsigned short)(~cksum);
}

int	send_packet(t_env *e)
{
	char datagram[PACKETSIZE];
	ssize_t sent_bytes;
	struct icmphdr *hdr = (struct icmphdr *)((void *)datagram);
	struct timeval *t_time =  (struct timeval *)((void *)datagram + sizeof(struct icmphdr));

	memset (datagram, 0, PACKETSIZE);
	hdr->type             = ICMP_ECHO;
	hdr->code             = 0;
	hdr->checksum         = 0;
	hdr->un.echo.id       = e->pid;
	hdr->un.echo.sequence = e->seq;
	if (gettimeofday(t_time, NULL))
	{
		printf("%s\n", "ERROR: interne: gettimeofday" );
		exit (EXIT_FAILURE);
	}
	ft_memcpy(datagram + (sizeof(struct icmphdr) + sizeof(struct timeval)), MY_DATA, ft_strlen(MY_DATA));
	hdr->checksum         = checksum((unsigned short *)datagram, PACKETSIZE);
	sent_bytes = sendto(e->socket, hdr, PACKETSIZE, 0,
		  (struct sockaddr *)e->ad_dst, sizeof(struct sockaddr));
	if (sent_bytes != PACKETSIZE)
	{
		printf("%s%d\n", "ERROR: interne: during send packet seq", e->seq);
		exit(EXIT_FAILURE);
	}
	else
	{
		e->nb_packet_send++;
	}
	return (EXIT_SUCCESS);
}

int	read_packet(t_env *e)
{
	unsigned char buf[1024];
    struct sockaddr_in addr;


    int bytes;
    unsigned int len = sizeof(addr);

     bzero(buf, sizeof(buf));
     bytes = recvfrom(e->socket, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr*)&addr, &len);
     if ( bytes > 0 )
	 {

		 struct icmphdr *hdr2 = (struct icmphdr *)((void *)buf + sizeof(struct ip));
		 struct timeval *t_time2 =  (struct timeval *)((void *)buf + sizeof (struct ip) + sizeof(struct icmphdr));

		 struct ip *iph2 = (struct ip *) buf;

		 struct timeval t_time;

		 if (gettimeofday(&t_time, NULL))
		 {
			 printf("%s\n", "ERROR: gettimeofday" );
			 exit (EXIT_FAILURE);
		 }

		 float t = ((1000000*t_time.tv_sec + t_time.tv_usec) - (1000000*t_time2->tv_sec + t_time2->tv_usec)) * 0.001;
		 int it = t;
		 float tt = it;
		 float r = t - it;

		 if (r >= 0.1)
		 {
			 printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%.1f ms\n", bytes - sizeof(struct ip), "str", hdr2->un.echo.sequence,  iph2->ip_ttl, t);
		 }
		 else
		 {
			 printf("%lu bytes from %s: icmp_seq=%d ttl=%d time=%d ms\n", bytes - sizeof(struct ip), "str", hdr2->un.echo.sequence,  iph2->ip_ttl, it);
		 }
		 e->nb_packet_rcv++;
		//  write(1, buf, bytes);
     }

	return (EXIT_SUCCESS);
}
