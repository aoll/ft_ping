/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buf_send_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/01 02:11:42 by alex              #+#    #+#             */
/*   Updated: 2017/12/01 02:12:03 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	buf_send_error(t_buf *buf, const char *err_mess, size_t size)
{
	if (ft_buf_add_data(buf, err_mess, size))
	{
		ft_buf_clean(buf);
		printf("ERROR: buf full\n");
		return (buf_send_error(buf, INTERN_ERROR, ft_strlen(INTERN_ERROR)));
	}
	return (EXIT_SUCCESS);
}
