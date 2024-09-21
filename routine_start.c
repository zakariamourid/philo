/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_start.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:48:27 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/21 22:49:39 by zmourid          ###   ########.fr       */
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

int	start_routine(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_init(&data->meals_lock, NULL);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->print, NULL);
	pthread_create(&data->architect, NULL, agent_smith, (void *)data);
	get_time();
	while (i < data->nbr)
	{
		pthread_create(&data->philos[i].thread, NULL, &matrix,
			(void *)&data->philos[i]);
		i++;
	}
	i = 0;
	while (i < data->nbr)
		pthread_join(data->philos[i++].thread, NULL);
	pthread_join(data->architect, NULL);
	destroy_all_mutexes(data);
	return (0);
}
