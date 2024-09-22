/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourid <zmourid@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 04:06:34 by zmourid           #+#    #+#             */
/*   Updated: 2024/09/22 00:36:58 by zmourid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

# define MUTEX pthread_mutex_t
# define LOCK pthread_mutex_lock
# define UNLOCK pthread_mutex_unlock
# define DESTROY pthread_mutex_destroy

typedef struct s_fork
{
	MUTEX			fork;
	int				id;
}					t_fork;

/* Shared ressources */
typedef struct s_data
{
	int				nbr;
	pthread_t		architect;
	long			begin;
	long			tt_die;
	long			tt_eat;
	long			tt_sleep;
	int				n_meals;
	t_fork			*forks;
	MUTEX			meals_lock;
	MUTEX			death;
	MUTEX			print;
	int				dead;
	int				full;
	struct s_philo	*philos;
}					t_data;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	t_fork			*right_fork;
	t_fork			*left_fork;
	long			last_meal_time;
	int				meal_count;
	t_data			*data;
	int				full;

}					t_philo;

long				ft_atoi(char *str);
long				get_time(void);
int					unlock_both_forks(t_philo *philo);
int					message(t_philo *philo, char *msg);
void				*agent_smith(void *param);
int					check_death(t_data *data);
void				ft_sleep(unsigned long mili_seconds, t_data *data);
int					start_routine(t_data *data);
void				*matrix(void *info);

#endif
