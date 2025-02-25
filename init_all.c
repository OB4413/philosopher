/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:43:59 by obarais           #+#    #+#             */
/*   Updated: 2025/02/25 15:24:06 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// init everything

// function for allocated memory
void	*ft_allocated(t_data *data)
{
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->num_philos);
	if (!data->forks)
		return NULL;
	data->philos = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->num_philos);
	if (!data->philos)
	{
		free(data->forks);
		return NULL;
	}
	return ("data");
}

// initialize data
int    init_the_data(t_data *data, char **av, int ac)
{
    int i;

    i = 0;
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->num_must_eat = INT_MAX;
	data->someone_died = 0;
	data->start_time = get_time();
	if (data->num_philos < 0 || data->num_philos > 200 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (printf("Error: some parameter is bad\n"), 1);
    
    // check if the number of times each philosopher must eat is given
	if (ac == 6)
	{
		data->num_must_eat = ft_atoi(av[5]);
		if (data->num_must_eat < 0)
			return (printf("Error: some parameter is bad\n"), 1);
	}
	
	// allocate memory for data.philos and data.forks
	if (ft_allocated(data) == NULL)
		return (printf("filed to allocate memory\n"), 1);
        
	// initialize mutex
	while (i < data->num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
    return (0);
}
