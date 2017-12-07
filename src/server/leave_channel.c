/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leave_channel.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 02:47:05 by alex              #+#    #+#             */
/*   Updated: 2017/12/01 03:02:54 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	leave_channel(t_env *e, int cs, char *cmd)
{
	if (ft_strlen(cmd + ft_strlen(LEAVE_CHANNEL) + 1))
	{
		buf_send_error(e->fds[cs].buf_write,
			LEAVE_CHANNEL_NO_ARGS, ft_strlen(LEAVE_CHANNEL_NO_ARGS));
		return (EXIT_FAILLURE);
	}
	ft_leave_chanel(e->channels, e->fds[cs].channel, cs);
	ft_bzero(e->fds[cs].channel, MAX_LEN_CHANEL_NAME);
	return (EXIT_SUCCESS);
}
