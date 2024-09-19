/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:29 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/19 04:43:15 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO

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
