/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 22:40:56 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/21 22:42:04 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat(t_philo *philo)
{
	LOCK(&philo->right_fork->fork);
	if (message(philo, "has taken a fork"))
	{
		UNLOCK(&philo->right_fork->fork);
		return (1);
	}
	if (philo->data->nbr == 1)
	{
		ft_sleep(philo->data->tt_die, philo->data);
		UNLOCK(&philo->right_fork->fork);
		return (1);
	}
	LOCK(&philo->left_fork->fork);
	if (message(philo, "has taken a fork"))
		return (unlock_both_forks(philo));
	LOCK(&philo->data->meals_lock);
	philo->last_meal_time = get_time();
	philo->meal_count++;
	UNLOCK(&philo->data->meals_lock);
	if (message(philo, "is eating"))
		return (unlock_both_forks(philo));
	ft_sleep(philo->data->tt_eat, philo->data);
	unlock_both_forks(philo);
	return (0);
}

int	_sleep(t_philo *philo)
{
	if (message(philo, "is sleeping"))
		return (1);
	ft_sleep(philo->data->tt_sleep, philo->data);
	return (0);
}

int	think(t_philo *philo)
{
	if (message(philo, "is thinking"))
		return (1);
	return (0);
}

void	*matrix(void *info)
{
	t_philo	*philo;

	philo = (t_philo *)info;
	if (philo->id % 2 == 0)
		ft_sleep(philo->data->tt_eat / 2, philo->data);
	while (1)
	{
		if (eat(philo))
			break ;
		if (_sleep(philo))
			break ;
		if (think(philo))
			break ;
		ft_sleep((philo->data->tt_die - (get_time() - philo->last_meal_time))
			/ 2, philo->data);
	}
	return (NULL);
}
