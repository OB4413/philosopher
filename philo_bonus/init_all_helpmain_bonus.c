/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all_helpmain_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:43:59 by obarais           #+#    #+#             */
/*   Updated: 2025/03/08 11:21:00 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_all(t_data *data)
{
	if (data->write_lock)
		sem_close(data->write_lock);
	if (data->forks)
		sem_close(data->forks);
	if (data->check_dead)
		sem_close(data->check_dead);
	if (data->meal_lock)
		sem_close(data->meal_lock);
	if (data->sameone_dead)
		sem_close(data->sameone_dead);
	sem_unlink("/sameone_dead");
	sem_unlink("/check_dead");
	sem_unlink("/forks");
	sem_unlink("/write_lock");
	sem_unlink("/meal_lock");
	free(data->philos);
}

int	help_main(t_data *data)
{
	int		i;
	int		stract;

	i = 0;
	while (i < data->num_philos)
	{
		waitpid(data->philos[i].pid, &stract, 0);
		i++;
	}
	return (0);
}

// function for allocated memory
void	*ft_allocated(t_data *data)
{
	data->philos = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->num_philos);
	if (data->philos == NULL)
		return (printf("malloc failed\n"), NULL);
	sem_unlink("/forks");
	sem_unlink("/write_lock");
	sem_unlink("/check_dead");
	sem_unlink("/meal_lock");
	sem_unlink("/sameone_dead");
	data->sameone_dead = sem_open("/sameone_dead", O_CREAT, 0644, 1);
	data->meal_lock = sem_open("/meal_lock", O_CREAT, 0644, 1);
	data->forks = sem_open("/forks", O_CREAT, 0644, data->num_philos);
	data->write_lock = sem_open("/write_lock", O_CREAT, 0644, 1);
	data->check_dead = sem_open("/check_dead", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->write_lock == SEM_FAILED
		|| data->check_dead == SEM_FAILED || data->meal_lock == SEM_FAILED)
		return (printf("sem_open failed\n"), NULL);
	return (data);
}

// initialize data
int	init_the_data(t_data *data, char **av, int ac)
{
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->num_must_eat = INT_MAX;
	data->start_time = get_time();
	if (data->num_philos <= 0 || data->num_philos > 200
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (printf("Error: some parameter is bad\n"), 1);
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
