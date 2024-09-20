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
#include <pthread.h>
#include <unistd.h>

void p_data(t_data *data)
{
    printf("number = %d -- tt_die %d -- tt_eat %d -- tt_sleep %d -- n_meals %d ", data->nbr,data->tt_die,data->tt_eat,data->tt_sleep,data->n_meals);
}
int init_data(t_data *data, char **av,int ac)
{
    //todo replace atoi by args check
    data->nbr = ft_atoi(av[1]);
    data->tt_die = ft_atoi(av[2]);
    data->tt_eat = ft_atoi(av[3]);
    data->tt_sleep = ft_atoi(av[4]);
    data->n_meals = -1;
    if(ac == 6)
        data->n_meals = ft_atoi(av[5]);
    return 0;
}
int take_forks(t_philo *philo)
{
    //f s;
//    if(philo->id == philo->data->nbr)
        philo->f_fork = &philo->data->forks[philo->id - 1];
        philo->s_fork = &philo->data->forks[philo->id % philo->data->nbr];
        return 0;
}

int init_philos(t_data *data)
{
    t_philo *philos;
    int i;
    int exit;

    i = 0;
    exit = 0;
    philos = malloc(sizeof(t_philo) * data->nbr);
    if(!philos)
        return 1;
    data->forks = malloc(sizeof(MUTEX) * data->nbr);
    if(!data->forks)
        return (free(philos),1);
    while(i < data->nbr)
    {
        philos[i].id = i + 1;
        philos[i].data = data;
        philos->f_fork = &data->forks[philos[i].id - 1];
        philos->s_fork = &data->forks[philos[i].id % philos[i].data->nbr];
        i++;
    }
    // if(exit) return(free(philos),free(data->forks),1);
    data->philos = philos;
    return 0;
}
void *simple_routine(void *num)
{
    int *n = (int *) num;
    printf("philo %d is created \n",*n);
    return NULL;
}
int start_routine(t_data *data)
{
    int i = 0;
    pthread_mutex_init(&data->lock, NULL);
    while(i < data->nbr)
    {
        pthread_create(&data->philos[i].thread,NULL,&simple_routine,(void *)&data->philos[i].id);
        usleep(50);
        i++;
    }
    i = 0;
    while(i < data->nbr)
        pthread_join(data->philos[i++].thread, NULL);
    printf("sf salina\n");
    pthread_mutex_destroy(&data->lock);
    return 1;
}
int main(int ac, char **av)
{
    t_data data;

    if(ac != 5 && ac != 6)
        return 0;
    if(init_data(&data, av,ac))
        return 1;
    if(init_philos(&data))
        return 1;
    if(start_routine(&data))
        return 1;
}
