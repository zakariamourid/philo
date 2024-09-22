/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:42:22 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/22 00:36:08 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(unsigned long mili_seconds, t_data *data)
{
	unsigned long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < mili_seconds)
	{
		LOCK(&data->death);
		if (data->dead)
		{
			UNLOCK(&data->death);
			break ;
		}
		UNLOCK(&data->death);
		usleep(500);
	}
}

long	get_time(void)
{
	static int				init;
	static struct timeval	start_time;
	struct timeval			now;

	if (init == 0)
	{
		gettimeofday(&start_time, NULL);
		init = 1;
	}
	gettimeofday(&now, NULL);
	return ((now.tv_sec - start_time.tv_sec) * 1000 + (now.tv_usec
			- start_time.tv_usec) / 1000);
}
