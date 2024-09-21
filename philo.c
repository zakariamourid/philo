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
int in_simulation(t_data *data)
{
    int finish;

    LOCK(&data->lock);
    finish = data->finish;
    UNLOCK(&data->lock);
    return finish;

}
void *matrix(void *info)
{
    t_philo *philo;

    philo = (t_philo * ) info;
    // printf("bsmlah id = %d\n",philo->id);
    while(!in_simulation(philo->data))
    {
        if(philo->data->n_meals != -1 && philo->meal_count == philo->data->n_meals)
        {
            printf("philo %d chba3 (meals => %d)\n",philo->id,philo->meal_count);
            break;
        }
        if(get_time() - philo->last_meal_time > philo->data->tt_die)
        {
            set_int(&philo->data->lock, &philo->data->finish, philo->id);
            break;
        }
        LOCK(&philo->right_fork->fork);
        printf("%ld %d has taken a fork\n", get_time(), philo->id);
        if(in_simulation(philo->data))
        {
            set_int(&philo->data->lock, &philo->data->finish, 1);
            UNLOCK(&philo->right_fork->fork);
        }
        LOCK(&philo->left_fork->fork);
        if(in_simulation(philo->data))
        {
            set_int(&philo->data->lock, &philo->data->finish, 1);
            UNLOCK(&philo->right_fork->fork);
            UNLOCK(&philo->left_fork->fork);
        }
        philo->last_meal_time = get_time();
        printf("%ld %d has taken a fork\n", get_time(), philo->id);
        printf("%ld %d is eating\n", get_time(), philo->id);
        philo->meal_count++;
        ft_sleep(philo->data->tt_eat, philo->data);
        UNLOCK(&philo->left_fork->fork);
        UNLOCK(&philo->right_fork->fork);
        printf("%ld %d is sleeping\n", get_time(), philo->id);
        ft_sleep(philo->data->tt_sleep, philo->data);
        printf("%ld %d is thinking\n", get_time(), philo->id);
        ft_sleep(philo->data->tt_think, philo->data);
    }
    return NULL;
}
int check_full(t_data *data)
{
    int all_full = 1;
    int i = 0;
    while(i < data->nbr)
    {
        LOCK(&data->lock);
        if(!data->philos[i].full)
            all_full = 0;
        UNLOCK(&data->lock);
        i++;
    }
    return all_full;
}
void *agent_smith(void *param)
{
    t_data *data;
    data = (t_data *)param;
    int i = 0;
    while(get_int(&data->lock, &data->finish) == 0 && check_full(data) != 1)
        ;
    int finish = get_int(&data->lock, &data->finish);
    if(finish)
        printf("philo %d is dead\n",finish);
    return NULL;
}
int start_routine(t_data *data)
{
    int i = 0;
    pthread_mutex_init(&data->lock, NULL);
    // pthread_create(&data->architect, NULL, agent_smith, (void *) data);
    get_time();
    while(i < data->nbr)
    {
        pthread_create(&data->philos[i].thread,NULL,&matrix,(void *)&data->philos[i]);
        i++;
    }
    i = 0;
    while(i < data->nbr)
        pthread_join(data->philos[i++].thread, NULL);
    // pthread_join(data->architect,  NULL);
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
    // p_data(&data);
    if(start_routine(&data))
        return 1;
    // printf("var= %d\n",data.var);
}
