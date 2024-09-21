/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:29 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/21 18:57:07 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

// TODO

unsigned long	get_time(void)
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
	return ((now.tv_sec - start_time.tv_sec) * 1000
		+ (now.tv_usec - start_time.tv_usec) / 1000);
}
int 	get_int(MUTEX *lock,int *n)
{
	int nbr;

	pthread_mutex_lock(lock);
	nbr = *n;
	pthread_mutex_unlock(lock);
	return nbr;
}

int 	set_int(MUTEX *lock, int *n,int value)
{
	LOCK(lock);
	*n = value;
	UNLOCK(lock);
	return value;
}

void	ft_sleep(unsigned long mili_seconds)
{
	unsigned long	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < mili_seconds)
		usleep(1);
}

int	ft_atoi(char *str)
{
	long	res;
	int		signe;
	res = 0;
	signe = 1;
	while (*str && *str == ' ')
		str++;
	if (*str && (*str == '-' || *str == '+'))
	{
		if (*str == '-')
			signe = -1;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		/*if (res * signe > INT_MAX || res * signe < INT_MIN)
		{
			clean_exit();
		}*/
		str++;
	}
	return ((int)res * signe);
}
