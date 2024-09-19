/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:34 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/19 05:08:33 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <mutex>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define MUTEX pthread_mutex_t
#define LOCK pthread_mutex_lock
#define UNLOCK pthread_mutex_unlock



/* Shared ressources */
typedef struct s_data
{
	int	nbr;
	int	tt_die;
	int tt_eat;
	int	tt_sleep;
	int	n_meals;
	MUTEX	*forks;
	struct s_philo	*philos;
} t_data;

typedef struct s_philo
{
	int id;
	pthread_t	thread;
	MUTEX *f_fork;
	MUTEX *s_fork;
	t_data	*data;
	
} t_philo;

int	ft_atoi(char *str);

#endif