/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_name.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 01:06:37 by alex              #+#    #+#             */
/*   Updated: 2017/12/01 04:26:01 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	set_name_check(t_buf *buf, const char *cmd)
{
	int	i;
	int	len;

	i = ft_strlen(NICKNAME);
	len = ft_strlen(cmd + ft_strlen(NICKNAME));
	if (!len)
	{
		buf_send_error(buf, NICKNAME_NEEDED, ft_strlen(NICKNAME_NEEDED));
		return (EXIT_FAILURE);
	}
	while (i < len - 1)
	{
		if (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == '\n' || cmd[i] == ' ')
		{
			buf_send_error(
				buf, NICKNAME_NOT_SPACE, ft_strlen(NICKNAME_NOT_SPACE));
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int			set_name(t_env *e, int cs, char *cmd)
{
	char	*trim;

	if (set_name_check(e->fds[cs].buf_write, cmd))
		return (EXIT_FAILURE);
	if (!(trim = ft_strtrim(cmd + ft_strlen(NICKNAME))))
	{
		buf_send_error(
			e->fds[cs].buf_write, INTERN_ERROR, ft_strlen(INTERN_ERROR));
		return (EXIT_FAILURE);
	}
	if (ft_strlen(trim) > MAX_LEN_LOGIN_NAME)
	{
		free(trim);
		buf_send_error(e->fds[cs].buf_write,
			NICKNAME_TO_LONG, ft_strlen(NICKNAME_TO_LONG));
		return (EXIT_FAILURE);
	}
	ft_bzero(e->fds[cs].name, MAX_LEN_LOGIN_NAME);
	ft_strcpy(e->fds[cs].name, trim);
	free(trim);
	return (EXIT_SUCCESS);
}
