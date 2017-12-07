/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_addr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/07 05:24:44 by alex              #+#    #+#             */
/*   Updated: 2017/12/07 07:17:33 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

struct sockaddr	*set_addr_dst(char *servname, char *str)
{
	int                  skt, errno;
    struct addrinfo      hints;  //prefered addr type(connection)
    struct addrinfo  *   list;   //list of addr structs
	// char				str[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags                = 0;
    hints.ai_family               = AF_INET;
    hints.ai_socktype             = SOCK_RAW;
    hints.ai_protocol             = IPPROTO_ICMP;

    if ((errno = getaddrinfo(servname, 0, &hints, &list))<0){
        fprintf(stderr, "addrinfo error, lookup fail:  %s", gai_strerror(errno));
        exit(1);
    }

	while (list)
	{
		if (inet_ntop(AF_INET, &((struct sockaddr_in *)list->ai_addr)->sin_addr, str, INET6_ADDRSTRLEN) == NULL) {
	        perror("inet_ntop");
	        // exit(EXIT_FAILURE);
	    }
		else
		{
			printf("str : %s\n", str);
			char host[NI_MAXHOST];
			getnameinfo(list->ai_addr, list->ai_addrlen, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
			printf("%s can be reached at %s\n", servname, host);
			return (list->ai_addr);
		}
		list = list->ai_next;
	}
	return (0);
}
