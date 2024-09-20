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
#include <string.h>
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
        philo->right_fork = &philo->data->forks[philo->id - 1];
        philo->left_fork = &philo->data->forks[philo->id % philo->data->nbr];
        return 0;
}

int init_forks(t_data *data)
{
    int i;

    i = 0;
    while(i < data->nbr)
    {
        if(pthread_mutex_init(&data->forks[i++], NULL))
            return 1;
    }
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
    int j = 0;
    while(j < data->nbr)
    {
        printf("fork %d add = %p\n",j, &data->forks[j]);
        j++;
    }
    if(init_forks(data))
        return 1;
    while(i < data->nbr)
    {
        philos[i].id = i + 1;
        philos[i].data = data;
        philos[i].right_fork = &data->forks[philos[i].id - 1];
        philos[i].left_fork = &data->forks[philos[i].id % data->nbr];
        philos[i].meal_count = 0;
        // printf("philo %d r  %d -- l %d\n",philos[i].id , philos[i].id - 1,philos[i].id % data->nbr);
        // printf("philo %d has right fork %p and left fork %p\n",philos[i].id, philos[i].right_fork, philos[i].left_fork);
        i++;
    }
    // if(exit) return(free(philos),free(data->forks),1);
    data->philos = philos;
    return 0;
}
void *simple_routine(void *info)
{
    t_philo *philo;

    philo = (t_philo * ) info;
    printf("id = %d\n",philo->id);
    while(1)
    {
        if(philo->meal_count > 5)
        {
            printf("philo %d chba3\n",philo->id);
            break;
        }
        pthread_mutex_lock(philo->right_fork);
        printf("%ld %d has taken a fork\n", get_time(), philo->id);
        pthread_mutex_lock(philo->left_fork);
        printf("%ld %d has taken a fork\n", get_time(), philo->id);
        printf("%ld %d is eating\n", get_time(), philo->id);
        ft_sleep(philo->data->tt_eat);
        philo->meal_count += 1;
        pthread_mutex_unlock(philo->left_fork);
        pthread_mutex_unlock(philo->right_fork);
        printf("%ld %d is sleeping\n", get_time(), philo->id);
        ft_sleep(philo->data->tt_sleep);
    }
    pthread_mutex_lock(&philo->data->lock);
    philo->data->finish = 1;
    pthread_mutex_unlock(&philo->data->lock);
    return NULL;
}
int start_routine(t_data *data)
{
    int i = 0;
    pthread_mutex_init(&data->lock, NULL);
    get_time();
    while(i < data->nbr)
    {
        pthread_create(&data->philos[i].thread,NULL,&simple_routine,(void *)&data->philos[i]);
        usleep(50);
        i++;
    }
    i = 0;
    while(i < data->nbr)
        pthread_join(data->philos[i++].thread, NULL);
    printf("sf salina\n");
    pthread_mutex_destroy(&data->lock);
    return 0;
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
    data.var = 0;
    if(start_routine(&data))
        return 1;
    // printf("var= %d\n",data.var);
}
