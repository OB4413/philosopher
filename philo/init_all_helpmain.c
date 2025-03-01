/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all_helpmain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:43:59 by obarais           #+#    #+#             */
/*   Updated: 2025/03/01 15:53:19 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	help_main(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_join(data->philo[i], NULL) != 0)
			return (printf("pthread_join failed\n"), 1);
		i++;
	}
	if (pthread_join(data->monitor, NULL) != 0)
		return (printf("pthread_join failed\n"), 1);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_destroy(&data->philos[i].meal_lock) != 0
			|| pthread_mutex_destroy(&data->forks[i]) != 0)
			return (printf("pthread_mutex_destroy failed\n"), 1);
		i++;
	}
	if (pthread_mutex_destroy(&data->write_lock) != 0
		|| pthread_mutex_destroy(&data->death_lock) != 0)
		return (printf("pthread_mutex_destroy failed\n"), 1);
	free(data->forks);
	free(data->philos);
	return (free(data->philo), 0);
}

// function for allocated memory
void	*ft_allocated(t_data *data)
{
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->num_philos);
	if (!data->forks)
		return (NULL);
	data->philos = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->num_philos);
	if (!data->philos)
	{
		free(data->forks);
		data->forks = NULL;
		return (NULL);
	}
	return ("data");
}

int	help_init(t_data *data)
{
	int	i;

	i = 0;
	if (ft_allocated(data) == NULL)
		return (printf("filed to allocate memory\n"), 1);
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (printf("pthread mutex init failed\n"), 1);
		i++;
	}
	data->philo = malloc(sizeof(pthread_t) * data->num_philos);
	if (!data->philo)
	{
		free(data->forks);
		free(data->philos);
		return (printf("failed to allocate memory\n"), 1);
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
	if (data->start_time == -1)
		return (1);
	if (ac == 6)
	{
		data->num_must_eat = ft_atoi(av[5]);
		if (data->num_must_eat < 0)
			return (printf("Error: some parameter is bad\n"), 1);
	}
	if (help_init(data) == 1)
		return (1);
	return (0);
}
