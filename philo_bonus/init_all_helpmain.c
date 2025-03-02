/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all_helpmain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:43:59 by obarais           #+#    #+#             */
/*   Updated: 2025/03/02 13:51:43 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	help_main(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		waitpid(&data->philos[i]);
		i++;
	}
	wait_pid(&data->pid_monitor);
	i = 0;
	while (i < data->num_philos)
	{
		sem_close(data->philos[i].meal_lock);
		sem_unlink("/meal_lock");
		i++;
	}
	sem_close(data->write_lock);
	sem_close(data->death_lock);
	sem_close(data->forks);
	sem_unlink("/forks");
	sem_unlink("/write_lock");
	sem_unlink("/death_lock");
	free(data->philos);
	return (0);
}

// function for allocated memory
void	*ft_allocated(t_data *data)
{
	data->forks = sem_open("/forks", O_CREAT, 0644, data->num_philos);
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

// initialize data
int	init_the_data(t_data *data, char **av, int ac)
{
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
	if (ft_allocated(data) == NULL)
		return (1);
	return (0);
}
