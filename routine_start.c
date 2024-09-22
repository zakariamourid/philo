/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_start.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:48:27 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/22 01:40:56 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_all_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr)
		DESTROY(&data->forks[i++].fork);
	DESTROY(&data->meals_lock);
	DESTROY(&data->death);
	DESTROY(&data->print);
}

int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->meals_lock, NULL))
		return (1);
	if (pthread_mutex_init(&data->death, NULL))
		return (1);
	if (pthread_mutex_init(&data->print, NULL))
		return (1);
	return (0);
}

int	start_routine(t_data *data)
{
	int	i;

	i = 0;
	if (init_mutexes(data))
		return (1);
	if (pthread_create(&data->architect, NULL, agent_smith, (void *)data))
		return (1);
	while (i < data->nbr)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &matrix,
				(void *)&data->philos[i]))
			return (1);
		i++;
	}
	i = 0;
	while (i < data->nbr)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(data->architect, NULL);
	destroy_all_mutexes(data);
	return (0);
}
