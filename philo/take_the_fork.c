/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/03/07 14:19:45 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	help_routine(t_philosopher *philo)
{
	if (put_the_fork(philo) == 1)
		return (1);
	if (check_death(philo))
		return (1);
	print_status(philo, "is sleeping");
	ft_usleep(philo, philo->data->time_to_sleep);
	return (0);
}

int	help_monitor(t_data *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_philos)
	{
		pthread_mutex_lock(&philo->meal_lock);
		if (get_time() - philo->philos[i].last_meal_time > philo->time_to_die)
		{
			print_status(&philo->philos[i], "is died");
			pthread_mutex_unlock(&philo->meal_lock);
			pthread_mutex_lock(&philo->death_lock);
			philo->someone_died = 1;
			pthread_mutex_unlock(&philo->death_lock);
			return (1);
		}
		pthread_mutex_unlock(&philo->meal_lock);
		i++;
	}
	return (0);
}

int	take_the_fork(t_philosopher *philo)
{
	int	right_fork;
	int	left_fork;

	left_fork = (philo->id + philo->data->num_philos - 1)
		% philo->data->num_philos;
	right_fork = philo->id % philo->data->num_philos;
	if ((philo->id + 1) % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		print_status(philo, "has taken a forks");
	}
	else
	{
		usleep(1000);
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		print_status(philo, "has taken a forks");
	}
	return (0);
}

int	put_the_fork(t_philosopher *philo)
{
	int	right_fork;
	int	left_fork;

	left_fork = (philo->id + philo->data->num_philos - 1)
		% philo->data->num_philos;
	right_fork = philo->id % philo->data->num_philos;
	if ((philo->id + 1) % 2 == 0)
	{
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
	}
	return (0);
}
