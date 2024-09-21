/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:34 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/21 02:15:45 by zmourid          ###   ########.fr       */
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


typedef struct s_fork
{
    MUTEX fork;
    int		id;
} t_fork;


/* Shared ressources */
typedef struct s_data
{
	int	nbr;
	pthread_t	architect;
	long begin;
	int	tt_die;
	int tt_eat;
	int	tt_sleep;
	int	n_meals;
	t_fork	*forks;
	MUTEX	lock;
	int		i;
	int	var;
	int	finish;
	long  tt_think;
	struct s_philo	*philos;
} t_data;

typedef struct s_philo
{
	int id;
	pthread_t	thread;
	t_fork *right_fork;
	t_fork *left_fork;
	long 	last_meal_time;
	int		meal_count;
	t_data	*data;
	int 	full;
	
} t_philo;


int	ft_atoi(char *str);
unsigned long	get_time(void);
int 	get_int(MUTEX *lock,int *n);
int 	set_int(MUTEX *lock, int *n,int value);
void	ft_sleep(unsigned long mili_seconds,t_data *data);

#endif