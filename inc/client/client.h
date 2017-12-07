/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/14 05:07:02 by alex              #+#    #+#             */
/*   Updated: 2017/12/05 16:23:02 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
# define CLIENT_H

# define PROMPT	"ft_p> "

# define INTERN_ERROR				"Intern error\n" //tmp
#include "ft_p.h"
#include "ft_irc.h"

int				start_client(char *addr, char *port);
int				create_client(char *addr, char *port, char *protocole);
struct addrinfo	*get_addrinfo(char *addr, char *port);
// int			switch_requet_client(int fd, char *requet);
// int			requet_put(int fd, char *requet);
// int			requet_get(int fd, char *requet);
// int			requet_cmd(int fd, char *requet, int output);
// int			quit_requet_client(int fd, char *requet);
// int			cd_requet_client(int fd, char *requet);
// int			requet_client(int fd, char *requet, int output);
// int			read_result_cmd(int fd, int output);
#endif
