/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/29 03:17:56 by alex              #+#    #+#             */
/*   Updated: 2017/12/01 00:19:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	clean_fd(t_fd *fd)
{
	fd->type = FD_FREE;
	fd->fct_read = NULL;
	fd->fct_write = NULL;
	ft_bzero(fd->name, MAX_LEN_LOGIN_NAME);
	ft_bzero(fd->channel, MAX_LEN_CHANEL_NAME);
}
