/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 17:14:02 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 01:07:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H


#define __USE_BSD	/* use bsd'ish ip header */
#include <sys/socket.h>	/* these headers are for a Linux system, but */
#include <netinet/in.h>	/* the names on other systems are easy to guess.. */
#include <netinet/ip.h>
#define __FAVOR_BSD	/* use bsd'ish tcp header */
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>



#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <sys/time.h>

#define PACKETSIZE_DATA 56
#define PACKETSIZE	64
#define PACKETSIZE_TOTAL 84

# include "libft.h"

#define MY_TTL 255
#define MY_DATA "abcdefghijklmnopqrstuvwxyzabcdefghijklmn"
#define MY_DATA_LEN 40

typedef struct	s_packet
{
	struct icmphdr	hdr;
	struct timeval	t_time;
	char			data[MY_DATA_LEN];
}	t_packet;

typedef struct	s_env
{
	int				seq;
	int				pid;
	float			min;
	float			max;
	float			total;
	float			mdev;
	int				nb_packet_send;
	int				nb_packet_rcv;
	int				nb_packet_error;
	int				socket;
	int				 option_v;
	int				option_t;
	char 			*adr;
	char			ipv4[INET_ADDRSTRLEN];
	char			*ai_canonname;
	struct sockaddr	*ad_dst;
	t_packet		pck;
}	t_env;

int				init_packet(t_env *e);
int				send_packet(t_env *e);
int				read_packet(t_env *e);

void			display(
	struct icmphdr *hdr2, struct timeval *t_time2, struct ip *iph2, t_env *e);

int				check_type(struct icmphdr *hdr, int bytes, t_env *e);
int				check_data(char *data, t_env *e);
int				check_sum(struct icmphdr *hdr2, t_env *e);
int				check_pid(struct icmphdr *hdr2, t_env *e);
int				check_size(int bytes, t_env *e);
unsigned short	checksum(unsigned short *buffer, int size);

#endif
