/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:29 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/21 22:42:49 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(char *str)
{
	long	res;

	res = 0;
	if (*str == 0)
		return (-1);
	if (*str && *str == '+')
		str++;
	while (*str && *str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		if (res > INT_MAX)
			return (-1);
		str++;
	}
	if (*str != 0)
		return (-1);
	return (res);
}

int	message(t_philo *philo, char *msg)
{
	LOCK(&philo->data->death);
	if (philo->data->dead)
	{
		UNLOCK(&philo->data->death);
		return (1);
	}
	UNLOCK(&philo->data->death);
	LOCK(&philo->data->print);
	printf("%ld %d %s\n", get_time(), philo->id, msg);
	UNLOCK(&philo->data->print);
	return (0);
}

int	unlock_both_forks(t_philo *philo)
{
	UNLOCK(&philo->left_fork->fork);
	UNLOCK(&philo->right_fork->fork);
	return (1);
}
