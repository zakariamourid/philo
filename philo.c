/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:43:37 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/21 22:48:06 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:38 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/20 02:42:07by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data, char **av, int ac)
{
	data->nbr = ft_atoi(av[1]);
	if (data->nbr == -1 || data->nbr == 0)
		return (1);
	data->tt_die = ft_atoi(av[2]);
	if (data->tt_die == -1 || data->tt_die == 0)
		return (1);
	data->tt_eat = ft_atoi(av[3]);
	if (data->tt_eat == -1 || data->tt_eat == 0)
		return (1);
	data->tt_sleep = ft_atoi(av[4]);
	if (data->tt_sleep == -1)
		return (1);
	data->n_meals = -1;
	data->dead = 0;
	data->full = 0;
	if (ac == 6)
		data->n_meals = ft_atoi(av[5]);
	return (0);
}

int	init_forks(t_fork *fork, int nbr)
{
	int	i;

	i = 0;
	while (i < nbr)
	{
		fork[i].id = i;
		if (pthread_mutex_init(&fork[i].fork, NULL))
			return (1);
		i++;
	}
	return (0);
}

int	take_forks(t_philo *philo)
{
	philo->right_fork = &philo->data->forks[philo->id - 1];
	philo->left_fork = &philo->data->forks[philo->id % philo->data->nbr];
	if (philo->data->nbr == philo->id)
	{
		philo->left_fork = &philo->data->forks[philo->id - 1];
		philo->right_fork = &philo->data->forks[philo->id % philo->data->nbr];
	}
	return (0);
}

int	init_philos(t_data *data)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = malloc(sizeof(t_philo) * data->nbr);
	if (!philos)
		return (1);
	data->forks = malloc(sizeof(t_fork) * data->nbr);
	if (!data->forks)
		return (free(philos), 1);
	if (init_forks(data->forks, data->nbr))
		return (free(philos), free(data->forks), 1);
	while (i < data->nbr)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		take_forks(&philos[i]);
		philos[i].meal_count = 0;
		philos[i].full = 0;
		philos[i].last_meal_time = get_time();
		i++;
	}
	data->philos = philos;
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		return (0);
	if (init_data(&data, av, ac))
	{
		printf("Error\n");
		return (1);
	}
	if (init_philos(&data))
		return (1);
	if (start_routine(&data))
		return (1);
	free(data.forks);
	free(data.philos);
}
