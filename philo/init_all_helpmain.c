/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all_helpmain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:43:59 by obarais           #+#    #+#             */
/*   Updated: 2025/03/07 14:33:16 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	help_main(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philo[i], NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->meal_lock);
	free(data->forks);
	free(data->philos);
	return (free(data->philo), 0);
}

// function for allocated memory
int	ft_allocated(t_data *data)
{
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->num_philos);
	if (!data->forks)
		return (1);
	data->philos = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->num_philos);
	if (!data->philos)
	{
		free(data->forks);
		data->forks = NULL;
		return (1);
	}
	return (0);
}

int	help_init(t_data *data)
{
	int	i;

	i = 0;
	if (ft_allocated(data) == 1)
		return (1);
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->meal_lock, NULL);
	data->philo = malloc(sizeof(pthread_t) * data->num_philos);
	if (!data->philo)
	{
		free(data->forks);
		free(data->philos);
		return (1);
	}
	return (0);
}

// initialize data
int	init_the_data(t_data *data, char **av, int ac)
{
	int	i;

	i = 0;
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->num_must_eat = INT_MAX;
	data->someone_died = 0;
	data->start_time = get_time();
	if (data->num_philos <= 0 || data->num_philos > 200
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (printf("Error: some parameter is bad\n"), 1);
	if (ac == 6)
	{
		data->num_must_eat = ft_atoi(av[5]);
		if (data->num_must_eat <= 0)
			return (printf("Error: some parameter is bad\n"), 1);
	}
	if (help_init(data) == 1)
		return (1);
	return (0);
}
