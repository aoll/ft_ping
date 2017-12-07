/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/05 15:35:26 by alex              #+#    #+#             */
/*   Updated: 2017/12/07 01:39:16 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <sys/time.h>


void	usage(const char *name)
{
	printf("Usage: %s <addr>\n", name);
	exit(EXIT_FAILLURE);
	return ;
}

int	create_sock(char *protocole)
{
	int					sock;
	struct protoent		*proto;

	if (!(proto = getprotobyname(protocole)))
		return (-1);
	// if ((sock = socket(PF_INET, SOCK_RAW, proto->p_proto)) == -1)
	if ((sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		return (-1);
	int one = 1;
	const int *val = &one;
	if (setsockopt (sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof (one)) < 0)
	{
		printf ("Warning: Cannot set HDRINCL!\n");
		return (-1);
	}
	return (sock);
}


int	read_sock(int sock)
{
	unsigned char		buf[1024];
	struct sockaddr_in	addr;
	char				ipstr[INET6_ADDRSTRLEN];
	int					bytes;
	socklen_t len;

	len = sizeof(addr);
	for (;;)
	{
		bzero(buf, sizeof(buf));
		bytes = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &len);
		if ( bytes > 0 )
			{
				printf("recv()'d %d bytes of data in buf: %s\n", bytes, buf);
				 struct ip *iph = (struct ip *) buf;

			printf("iph->ip_off : %d - iph->ip_ttl : %d - iph->ip_p : %d - iph->ip_sum : %d\n -	iph->ip_tos : %d",
			iph->ip_off, iph->ip_ttl, iph->ip_p, iph->ip_sum, iph->ip_tos);

				printf("from IP address %s\n",
    				inet_ntop(addr.sin_family,
        						// addr.ss_family == AF_INET ?
            					(const void *)&(addr.sin_addr)
								// :
            					// ((struct sockaddr_in6 *)&addr)->sin6_addr
								,
        						ipstr, sizeof ipstr));
			}
		else
			perror("recvfrom");
		return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}

#include <errno.h>
#define  PING_BUF_SIZE 4096


int	rcv_sock(int sock, char *address)
{
	struct msghdr	msg;
	ssize_t			bytes_rcv;
	char			buf[PING_BUF_SIZE];
	char			name[PING_BUF_SIZE];
	char			buf_ctrl[PING_BUF_SIZE];
	struct iovec 	iov;
	struct cmsghdr	*cmsg;

	ft_bzero((void *)&msg, sizeof(msg));
	ft_bzero((void *)&msg.msg_iov, sizeof(msg.msg_iov));

	bzero(buf, PING_BUF_SIZE);
	bzero(name, PING_BUF_SIZE);
	bzero(buf_ctrl, PING_BUF_SIZE);
	msg.msg_name = name;
	msg.msg_namelen = PING_BUF_SIZE;
	msg.msg_control = buf_ctrl;
	msg.msg_controllen = PING_BUF_SIZE;
	//
	iov.iov_base = buf;
	iov.iov_len  = PING_BUF_SIZE;

	msg.msg_iov        = &iov;
	msg.msg_iovlen     = 1;


	// printf("msg.msg_namelen: %d - msg.msg_controllen: %zu\n", msg.msg_namelen, msg.msg_controllen);

	bytes_rcv = recvmsg(sock, &msg, 0);
	if (bytes_rcv != -1)
	{
		// printf("bytes_rcv: %zd\n", bytes_rcv);
		// printf("msg.msg_namelen is back: %d - msg.msg_name: %s - msg.msg_controllen: %zu\n", msg.msg_namelen, msg.msg_name, msg.msg_controllen);
		// printf("msg.msg_iov->iov_base: %s - msg.msg_iov->iov_len: %zu\n", msg.msg_iov[0].iov_base, msg.msg_iov[0].iov_len);
		// printf("msg.msg_iovlen: %zu\n", msg.msg_iovlen);

		struct sockaddr_in	*addr;
		char				ipstr[INET6_ADDRSTRLEN];

		addr = (struct sockaddr_in *)msg.msg_name;
		// printf("(struct sockaddr_in *): %zu\n", sizeof(struct sockaddr_in *));
		// printf("from IP address %s\n",
		// 	inet_ntop(addr->sin_family,
		// 				// addr->ss_family == AF_INET ?
		// 				(const void *)&(addr->sin_addr)
		// 				// :
		// 				// ((struct sockaddr_in6 *)&addr)->sin6_addr
		// 				,
		// 				ipstr, sizeof ipstr));

			struct ip *iph = (struct ip *) msg.msg_iov[0].iov_base;

			// printf("from IP address iph->ip_src.s_addr %s\n",
			// 	inet_ntop(addr->sin_family,
			// 				// addr->ss_family == AF_INET ?
			// 				(const void *)&(iph->ip_dst.s_addr)
			// 				// :
			// 				// ((struct sockaddr_in6 *)&addr)->sin6_addr
			// 				,
			// 				ipstr, sizeof ipstr));


				// 			printf("iph->ip_off : %d - iph->ip_ttl : %d - iph->ip_p : %d - iph->ip_sum : %d\n -	iph->ip_tos : %d",
				// 			iph->ip_off, iph->ip_ttl, iph->ip_p, iph->ip_sum, iph->ip_tos);

				struct icmphdr *pkt = (struct icmphdr *) ((void *)msg.msg_iov[0].iov_base + sizeof (struct ip));
				// printf("pkt->type: %d - pkt->code: %d - pkt->checksum: %d - pkt->un.echo.id: %d - pkt->un.echo.sequence:%d\n",
				// pkt->type, pkt->code, pkt->checksum, pkt->un.echo.id, pkt->un.echo.sequence);

			struct cmsghdr *cm = (struct cmsghdr *)msg.msg_control;

			// printf("cmsg_len: %zu - cmsg_level: %d - cmsg_type: %d\n", cm->cmsg_len, cm->cmsg_level, cm->cmsg_type);


			struct timeval *t_time = (struct timeval *) ((void *)msg.msg_iov[0].iov_base + sizeof(*iph) + sizeof(*pkt) );

			struct timeval t_time_now;

			if (gettimeofday(&t_time_now, NULL))
			{
				printf("%s\n", "ERROR: gettimeofday" );
				exit (EXIT_FAILLURE);
			}

			printf("%zd bytes from %s: icmp_seq=%d ttl=%d time=%f ms\n", bytes_rcv, address, pkt->un.echo.sequence, iph->ip_ttl, (t_time_now.tv_usec - t_time->tv_usec) * 0.001);
			// printf("--------------------2--------------------time->tv_sec: %ld - time->tv_usec: %ld\n", t_time->tv_sec, t_time->tv_usec);


	}
	else
	{
		printf("Oh dear, something went wrong with read()! %s\n", strerror(errno));
		printf("%s\n", "Fail");
	}
	return (EXIT_SUCCESS);
}

uint16_t ip_checksum(void* vdata,size_t length) {
    // Cast the data pointer to one that can be indexed.
    char* data=(char*)vdata;

    // Initialise the accumulator.
    uint32_t acc=0xffff;

    // Handle complete 16-bit blocks.
    for (size_t i=0;i+1<length;i+=2) {
        uint16_t word;
        memcpy(&word,data+i,2);
        acc+=ntohs(word);
        if (acc>0xffff) {
            acc-=0xffff;
        }
    }

    // Handle any partial block at the end of the data.
    if (length&1) {
        uint16_t word=0;
        memcpy(&word,data+length-1,1);
        acc+=ntohs(word);
        if (acc>0xffff) {
            acc-=0xffff;
        }
    }

    // Return the checksum in network byte order.
    return htons(~acc);
}

#define P 25		/* lets flood the sendmail port */

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

int	ping(char *addr_s, char *port, int sock)
{
	struct sockaddr_in	addrDest;
	struct sockaddr_in	addrLocal;
	struct icmphdr		icmp;
	char				*msg = ft_strdup("Hello World");
	char buf[256];

	addrDest.sin_family = AF_INET;
	addrDest.sin_port = htons(8888);
	addrDest.sin_addr.s_addr = inet_addr(addr_s);


	icmp.type = 8;
	icmp.type = 0;
	icmp.checksum = 0;
	icmp.un.echo.id = 1;
	icmp.un.echo.sequence = 1;


	const size_t req_size=8;
	struct icmphdr req;
	req.type=8;
	req.code=0;
	req.checksum=0;
	req.un.echo.id=htons(rand());
	req.un.echo.sequence=htons(1);
	req.checksum=ip_checksum(&req,req_size);

	memcpy(buf, &req, sizeof(req));
	// strlen(msg)+1 for terminating null char
	int result = sendto(sock, buf, 256, 0, (struct sockaddr*)&addrDest, sizeof(addrDest));
	if (result == -1)
	{
		printf("Oh dear, something went wrong with sendto()! %s\n", strerror(errno));
		printf("%s\n", "ERROR: sendto");
		exit(EXIT_FAILLURE);
	}
	// printf("sentdo: %d\n", result);
	return (EXIT_SUCCESS);
}


int
main (int ac, char **av)
{
	int pid = getpid();
  int s = socket (PF_INET, SOCK_RAW, IPPROTO_ICMP);	/* open raw socket */
// inet_pton(AF_INET, addr, serveraddr);

	printf("IPPROTO_ICMP:%d\n", IPPROTO_ICMP);
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_port = htons (P);/* you byte-order >1byte header values to network
    			  byte order (not needed on big endian machines) */
    // sin.sin_addr.s_addr = inet_addr (av[1]);
	if (inet_pton(AF_INET, av[1], &sin.sin_addr.s_addr) != 1)
	{
		printf("%s\n", "ERROR: inet_pton");
		exit (EXIT_FAILLURE);
	}
	int seq = 0;
	int send = 0;
	while (1)
	{

  int one = 1;
  const int *val = &one;
  if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, &one, sizeof (one)) < 0)
  {
  	printf ("Warning: Cannot set HDRINCL!\n");
  	return (-1);
  }
  char datagram[4096];	/* this buffer will contain ip header, icmp header,
			   and payload. we'll point an ip header structure
			   at its beginning, and a icmp header structure after
			   that to write the header values into it */
  struct ip *iph = (struct ip *) datagram;

	// printf("struct ip : %zu, icmphdr: %zu, timeval: %zu\n", sizeof(struct ip), sizeof(struct icmphdr), sizeof(struct timeval));

	struct timeval *t_time =  (struct timeval *)((void *)datagram + sizeof (struct ip) + sizeof(struct icmphdr));

	if (gettimeofday(t_time, NULL))
	{
		printf("%s\n", "ERROR: gettimeofday" );
		exit (EXIT_FAILLURE);
	}
	// printf("---------------------time->tv_sec: %ld - time->tv_usec: %ld\n", t_time->tv_sec, t_time->tv_usec);

  struct icmphdr *pkt = (struct icmphdr *) ((void *)datagram + sizeof (struct ip));
                pkt->type = ICMP_ECHO;
                pkt->code = 0;
                pkt->checksum = 0;
                pkt->un.echo.id = htons(pid & 0xFFFF);
                pkt->un.echo.sequence = seq++;
                pkt->checksum = csum ((unsigned short *) pkt, sizeof(*pkt) >> 1);

/* we'll now fill in the ip header values, see above for explanations */
  iph->ip_hl = 5;
  iph->ip_v = 4;
  iph->ip_tos = 0;
  iph->ip_len = sizeof (struct ip) + sizeof(*pkt) + sizeof(struct timeval);	/* no payload */
  iph->ip_id = 0;	/* the value doesn't matter here */
  iph->ip_off = 0;
  iph->ip_ttl = 64;
  iph->ip_p = IPPROTO_ICMP;
  iph->ip_sum = 0;		/* set it to 0 before computing the actual checksum later */

  iph->ip_src.s_addr = inet_addr ("0.0.0.0");/* SYN's can be blindly spoofed */
  // iph->ip_src.s_addr = inet_addr ("127.0.0.1");/* SYN's can be blindly spoofed */
  iph->ip_dst.s_addr = sin.sin_addr.s_addr;
  // iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);



      if ((send = sendto (s,		/* our socket */
		  datagram,	/* the buffer containing headers and data */
		  64,	/* total length of our datagram */
		  0,		/* routing flags, normally always 0 */
		  (struct sockaddr *) &sin,	/* socket addr, just like in */
		  sizeof (sin))) < 0)		/* a normal send() */
		  {

			  printf ("error\n");
		  }
      else
	  {
		  printf("send: %d\n", send);
		//   read_sock(s);
		  rcv_sock(s, av[1]);
	  }
	  sleep(1);
    }

  return 0;
}

// int	main(int ac, char **av)
// {
// 	int					sock;
// 	struct addrinfo		*res;
//
// 	if (ac != 2)
// 		usage(av[0]);
// 	if ((sock = create_sock("ICMP")) == -1)
// 	{
// 		printf("%s\n", "ERROR: create_client");
// 		exit(EXIT_FAILLURE);
// 	}
// 	if (ping(av[1], "0", sock))
// 		return (-1);
// 	// rcv_sock(sock);
// 	read_sock(sock);
// 	printf("%s\n", "SUCCESS");
// 	return (EXIT_SUCCESS);
// }
