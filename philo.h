/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:34 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/20 18:43:46 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
# include <sys/time.h>
#include <limits.h>

#define MUTEX pthread_mutex_t
#define LOCK pthread_mutex_lock
#define UNLOCK pthread_mutex_unlock



/* Shared ressources */
typedef struct s_data
{
	int	nbr;
	unsigned long begin;
	int	tt_die;
	int tt_eat;
	int	tt_sleep;
	int	n_meals;
	MUTEX	*forks;
	MUTEX	lock;
	int		i;
	int	var;
	int	finish;
	struct s_philo	*philos;
} t_data;

typedef struct s_philo
{
	int id;
	pthread_t	thread;
	MUTEX *right_fork;
	MUTEX *left_fork;
	unsigned long 	last_meal_time;
	int		meal_count;
	t_data	*data;
	
} t_philo;

int	ft_atoi(char *str);
unsigned long	get_time(void);
void	ft_sleep(unsigned long mili_seconds);

#endif