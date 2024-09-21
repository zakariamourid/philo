/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   architect.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:40:25 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/21 22:40:35 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_full(t_data *data)
{
	int		all_full;
	t_philo	*philo;
	int		i;

	all_full = 1;
	i = 0;
	while (i < data->nbr)
	{
		philo = &data->philos[i];
		LOCK(&data->meals_lock);
		if (philo->meal_count < data->n_meals)
			all_full = 0;
		UNLOCK(&data->meals_lock);
		i++;
	}
	if (all_full == 1)
	{
		LOCK(&data->death);
		data->dead = 1;
		UNLOCK(&data->death);
	}
	return (all_full);
}

int	check_death(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr)
	{
		LOCK(&data->meals_lock);
		if (get_time() - data->philos[i].last_meal_time > data->tt_die)
		{
			printf("%ld %d died\n", get_time(), data->philos[i].id);
			LOCK(&data->death);
			data->dead = 1;
			UNLOCK(&data->death);
			UNLOCK(&data->meals_lock);
			return (1);
		}
		UNLOCK(&data->meals_lock);
		i++;
	}
	return (0);
}

void	*agent_smith(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	while (1)
	{
		if (check_death(data) != 0)
			break ;
		if (data->n_meals != -1 && check_full(data) != 0)
			break ;
	}
	return (NULL);
}
