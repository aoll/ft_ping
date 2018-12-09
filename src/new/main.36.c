/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 16:58:49 by alex              #+#    #+#             */
/*   Updated: 2018/12/08 00:23:39 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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











#define PACKETSIZE	64












#define P 25		/* lets flood the sendmail port */

#define IDENTIFIER 0x2342



unsigned short		/* this function generates header checksums */
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

int
main (void)
{
 int sequenceno = 0;
 int pid = getpid();
  int s = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP);	/* open raw socket */

  const int val=255;
  if ( setsockopt(s, SOL_IP, IP_TTL, &val, sizeof(val)) != 0) {
   printf("%s\n", "ERROR");
   return 1;
  }

  char datagram[PACKETSIZE];	/* this buffer will contain ip header, tcp header,
			   and payload. we'll point an ip header structure
			   at its beginning, and a tcp header structure after
			   that to write the header values into it */
  memset (datagram, 0, PACKETSIZE);	/* zero out the buffer */
  struct ip *iph = (struct ip *) datagram;



  struct icmphdr *hdr = (struct icmphdr *)((void *)datagram + sizeof(struct ip));


  hdr->type             = ICMP_ECHO;         /* ICMP echo request */
  hdr->code             = 0;
  hdr->checksum         = 0;  /* required for actual checksum calculation */
  hdr->un.echo.id       = htons(IDENTIFIER); /* identifier  */  // must be pid
  hdr->un.echo.sequence = htons(sequenceno); /* sequence no */

  // datagram[28] = 'a';

  // struct timeval *t_time =  (struct timeval *)((void *)datagram + sizeof(struct ip) + sizeof(struct icmphdr));
  struct timeval *t_time =  (struct timeval *)((void *)datagram + 28);
  printf("sizeof hdr 0 : %d\n", sizeof(struct icmphdr));
  printf("sizeof time 0 : %d\n", sizeof(struct timeval));
 	if (gettimeofday(t_time, NULL))
 	{
 		printf("%s\n", "ERROR: gettimeofday" );
 		exit (1);
 	}

  printf("time 0 %d\n", t_time->tv_usec);



  // for (size_t i = 0; i < count; i++) {
  //  /* code */
  // }
  /* Fill in the ICMP checksum */
  hdr->checksum         = csum((uint16_t *)hdr, 16);
  // hdr->checksum         = csum((uint16_t *)hdr, sizeof(hdr) + sizeof(t_time));


  //

  struct sockaddr_in sin;
			/* the sockaddr_in containing the dest. address is used
			   in sendto() to determine the datagrams path */

  sin.sin_family = AF_INET;
  sin.sin_port = htons (P);/* you byte-order >1byte header values to network
			      byte order (not needed on big endian machines) */
  sin.sin_addr.s_addr = inet_addr ("216.58.204.100");



  printf("hdr %d\n", sizeof(hdr));
  printf("icmphdr %d\n", sizeof(struct icmphdr));
  printf("ip %d\n", sizeof(struct ip));

  // size_t i = sizeof(struct ip) + sizeof(struct icmphdr);
  // datagram[28] = 'a';
  // hdr->checksum         = csum((uint16_t *)hdr, sizeof(hdr) + 1);

  const ssize_t sent_bytes =
    sendto(s,
           // datagram, 29,
           hdr, 36,
           // hdr, sizeof(hdr) + sizeof(t_time),
           0 /* no MSG_* flags */,
           (struct sockaddr *)&sin, sizeof(sin));
  if (sent_bytes != 36) {
  // if (sent_bytes != sizeof(hdr) + sizeof(t_time)) {
  // if (sent_bytes != 29) {
    printf("%s\n", "ERROR");
  }
  else
  {
   printf("%s\n", "SUCCESS 2");
  }



  unsigned char buf[1024];
  struct sockaddr_in addr;


  	int bytes, len=sizeof(addr);

   bzero(buf, sizeof(buf));
   bytes = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &len);
   if ( bytes > 0 ){

    printf("RECV !! : %s, %d\n", buf, bytes);
   }
   else
    {
     printf("%s\n", "error");
     return 1;
    }

    struct icmphdr *hdr2 = (struct icmphdr *)((void *)buf + sizeof(struct ip));
    printf("type: %d\n",  hdr2->type );

    printf("ICMP_ECHO: %d\n", ICMP_ECHO );

    struct timeval *t_time2 =  (struct timeval *)((void *)buf + sizeof (struct ip) + sizeof(struct icmphdr));

    printf("time 1 %d\n", t_time2->tv_usec);

    struct ip *iph2 = (struct ip *) buf;

    printf("ttl %d\n", iph2->ip_ttl);

  close(s);





// /* we'll now fill in the ip/tcp header values, see above for explanations */
//   iph->ip_hl = 5;
//   iph->ip_v = 4;
//   iph->ip_tos = 0;
//   iph->ip_len = sizeof (struct ip) + sizeof (struct tcphdr);	/* no payload */
//   iph->ip_id = htonl (54321);	/* the value doesn't matter here */
//   iph->ip_off = 0;
//   iph->ip_ttl = 255;
//   iph->ip_p = 6;
//   iph->ip_sum = 0;		/* set it to 0 before computing the actual checksum later */
//   iph->ip_src.s_addr = inet_addr ("1.2.3.4");/* SYN's can be blindly spoofed */
//   iph->ip_dst.s_addr = sin.sin_addr.s_addr;
//   tcph->th_sport = htons (1234);	/* arbitrary port */
//   tcph->th_dport = htons (P);
//
//   tcph->th_seq = random ();/* in a SYN packet, the sequence is a random */
//   tcph->th_ack = 0;/* number, and the ack sequence is 0 in the 1st packet */
//   tcph->th_x2 = 0;
//   tcph->th_off = 0;		/* first and only tcp segment */
//   tcph->th_flags = TH_SYN;	/* initial connection request */
//   tcph->th_win = htonl (65535);	/* maximum allowed window size */
//   tcph->th_sum = 0;/* if you set a checksum to zero, your kernel's IP stack
// 		      should fill in the correct checksum during transmission */
//   tcph->th_urp = 0;
//
//   iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);
//
// /* finally, it is very advisable to do a IP_HDRINCL call, to make sure
//    that the kernel knows the header is included in the data, and doesn't
//    insert its own header into the packet before our data */
//
//   // {				/* lets do it the ugly way.. */
//     int one = 1;
//     const int *val = &one;
//     if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
//       printf ("Warning: Cannot set HDRINCL!\n");
//       return 1;
//   // }
//
//   while (1)
//     {
//       if (sendto (s,		/* our socket */
// 		  datagram,	/* the buffer containing headers and data */
// 		  iph->ip_len,	/* total length of our datagram */
// 		  0,		/* routing flags, normally always 0 */
// 		  (struct sockaddr *) &sin,	/* socket addr, just like in */
// 		  sizeof (sin)) < 0)		/* a normal send() */
// 	printf ("error\n");
//       else
// 	printf (".");
//     }

  return 0;
}
