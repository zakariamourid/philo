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
    printf("number = %d -- tt_die %d -- tt_eat %d -- tt_sleep %d -- n_meals %d  ", data->nbr,data->tt_die,data->tt_eat,data->tt_sleep,data->n_meals);
}
int init_data(t_data *data, char **av,int ac)
{
    //todo replace atoi by args check
    data->nbr = ft_atoi(av[1]);
    data->tt_die = ft_atoi(av[2]);
    data->tt_eat = ft_atoi(av[3]);
    data->tt_sleep = ft_atoi(av[4]);
    data->n_meals = -1;
    data->finish = 0;
    data->dead = 0;
    if (data->nbr % 2 && data->tt_eat >= data->tt_sleep)
        data->tt_think = data->tt_eat * 2 - data->tt_sleep;
    else
        data->tt_think = 0;
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

int init_forks(t_fork *fork,int nbr)
{
    int i;

    i = 0;
    while(i < nbr)
    {
        fork[i].id = i;
        // printf("fork id = %d -- add = %p\n",fork[i].id,&fork[i].fork);
        if(pthread_mutex_init(&fork[i].fork, NULL))
            return 1;
        i++;
    }
    return 0;
}
int init_philos(t_data *data)
{
    t_philo *philos;
    int i;

    i = 0;
    philos = malloc(sizeof(t_philo) * data->nbr);
    if(!philos)
        return 1;
    data->forks = malloc(sizeof(t_fork) * data->nbr);
    if(!data->forks)
        return (free(philos),1);
    if(init_forks(data->forks,data->nbr))
        return 1;
    while(i < data->nbr)
    {
        philos[i].id = i + 1;
        philos[i].data = data;
        philos[i].right_fork = &data->forks[philos[i].id - 1];
        philos[i].left_fork = &data->forks[philos[i].id % data->nbr];
        if(i == data->nbr - 1)
        {
            philos[i].left_fork = &data->forks[philos[i].id - 1];
            philos[i].right_fork = &data->forks[philos[i].id % data->nbr];
        }
        philos[i].meal_count = 0;
        philos[i].full = 0;
        philos[i].last_meal_time = get_time();
        //  printf("philo %d r  %d -- l %d\n",philos[i].id , philos[i].right_fork->id,philos[i].left_fork->id);
        //  printf("philo %d has right fork %p and left fork %p\n",philos[i].id, &philos[i].right_fork->fork, &philos[i].left_fork->fork);
        i++;
    }
    data->philos = philos;
    return 0;
}


int message(t_philo *philo,char *msg)
{
    LOCK(&philo->data->death);
    if(philo->data->dead)
    {
        UNLOCK(&philo->data->death);
        return 1;
    }
    UNLOCK(&philo->data->death);
    LOCK(&philo->data->print);
    printf("%ld %d %s\n",get_time(),philo->id,msg);
    UNLOCK(&philo->data->print);
    return 0;
}
int eat(t_philo *philo)
{
    
    LOCK(&philo->right_fork->fork);
    if(message(philo,"has taken a fork"))
    {
        UNLOCK(&philo->right_fork->fork);
        return 1;
    }
    LOCK(&philo->left_fork->fork);
    if(message(philo,"has taken a fork"))
    {
        UNLOCK(&philo->left_fork->fork);
        UNLOCK(&philo->right_fork->fork);
        return 1;
    }
    LOCK(&philo->data->meals_lock);
    philo->last_meal_time = get_time();
    philo->meal_count++;
    UNLOCK(&philo->data->meals_lock);
    if(message(philo,"is eating"))
    {
        UNLOCK(&philo->left_fork->fork);
        UNLOCK(&philo->right_fork->fork);
        return 1;
    }
    ft_sleep(philo->data->tt_eat);
    UNLOCK(&philo->left_fork->fork);
    UNLOCK(&philo->right_fork->fork);
    return 0;
}
int _sleep(t_philo *philo)
{
    if(message(philo, "is sleeping"))
        return 1;
    ft_sleep(philo->data->tt_sleep);
    return 0;
}
int think(t_philo *philo)
{
    if(message(philo, "is thinking"))
        return 1;
    // ft_sleep(philo->data->tt_think);
    return 0;
}
void *matrix(void *info)
{
    t_philo *philo;

    philo = (t_philo * ) info;
    while(1)
    {
        if(eat(philo))
            break ;
        if(_sleep(philo))
            break;
        if(think(philo))
            break;
        // printf("%ld last meal time --- id = %d\n",philo->last_meal_time,philo->id);
        ft_sleep((philo->data->tt_die - (get_time() - philo->last_meal_time)) / 2);
    }
    return NULL;
}
int check_full(t_data *data)
{
    printf("checking fllllllllllllllllllllllllllllllllllll\n");
    int all_full = 1;
    int i = 0;
    if(data->n_meals == -1)
        return 1;
    while(i < data->nbr)
    {
        printf("philo %d kal %d\n",data->philos[i].id , data->philos[i].meal_count);
        LOCK(&data->meals_lock);
        if(data->philos[i].meal_count < data->n_meals)
            all_full = 0;
        UNLOCK(&data->meals_lock);
        i++;
    }
    return all_full;
}
int check_death(t_data *data)
{
    int i = 0;
    while(i < data->nbr)
    {
        LOCK(&data->meals_lock);
        if(get_time() - data->philos[i].last_meal_time > data->tt_die)
        {
            printf("%ld %d died\n", get_time(), data->philos[i].id);
            LOCK(&data->death);
            data->dead = 1;
            UNLOCK(&data->death);
            UNLOCK(&data->meals_lock);
            return 1;
        }
        UNLOCK(&data->meals_lock);
    }
    return 0;
}
void *agent_smith(void *param)
{
    t_data *data;

    data = (t_data *)param;
    while(check_death(data) == 0 && check_full(data) != 1)
        ;
    return NULL;
}
int start_routine(t_data *data)
{
    int i = 0;	
    pthread_mutex_init(&data->meals_lock, NULL);
    pthread_mutex_init(&data->death, NULL);
    pthread_mutex_init(&data->print, NULL);
    pthread_create(&data->architect, NULL, agent_smith, (void *) data);
    get_time();
    while(i < data->nbr)
    {
        pthread_create(&data->philos[i].thread,NULL,&matrix,(void *)&data->philos[i]);
        i++;
    }
    i = 0;
    while(i < data->nbr)
        pthread_join(data->philos[i++].thread, NULL);
    pthread_join(data->architect,  NULL);
    pthread_mutex_destroy(&data->meals_lock);
    pthread_mutex_destroy(&data->death);
    pthread_mutex_destroy(&data->print);
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
    if(start_routine(&data))
        return 1;
}
