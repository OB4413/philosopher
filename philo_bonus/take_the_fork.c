/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/03/01 14:10:56 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	help_routine(t_philosopher *philo)
{
	if (put_the_fork(philo) == 1)
		return (1);
	if (check_death(philo))
		return (1);
	return (0);
}

int	help_monitor(t_data *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_philos)
	{
		if (pthread_mutex_lock(&philo->philos[i].meal_lock) != 0)
			return (printf("pthread_mutex_lock failed\n"), 1);
		if (get_time() - philo->philos[i].last_meal_time > philo->time_to_die)
		{
			print_status(&philo->philos[i], "is died");
			if (pthread_mutex_unlock(&philo->philos[i].meal_lock) != 0)
				return (printf("pthread_mutex_unlock failed\n"), 1);
			if (pthread_mutex_lock(&philo->death_lock) != 0)
				return (printf("pthread_mutex_lock failed\n"), 1);
			philo->someone_died = 1;
			if (pthread_mutex_unlock(&philo->death_lock) != 0)
				return (printf("pthread_mutex_unlock failed\n"), 1);
			return (1);
		}
		if (pthread_mutex_unlock(&philo->philos[i].meal_lock) != 0)
			return (printf("pthread_mutex_unlock failed\n"), 1);
		i++;
	}
	return (0);
}

int	help_take_fork(t_philosopher *philo, int left_fork, int right_fork)
{
	if (check_death(philo))
		return (1);
	usleep(2000);
	if (pthread_mutex_lock(&philo->data->forks[right_fork]) != 0)
		return (printf("pthread_mutex_lock failed\n"), 1);
	print_status(philo, "has taken a fork");
	if (pthread_mutex_lock(&philo->data->forks[left_fork]) != 0)
		return (printf("pthread_mutex_lock failed\n"), 1);
	print_status(philo, "has taken a fork");
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
		if (check_death(philo))
			return (1);
		if (pthread_mutex_lock(&philo->data->forks[left_fork]) != 0)
			return (printf("pthread_mutex_lock failed\n"), 1);
		print_status(philo, "has taken a fork");
		if (pthread_mutex_lock(&philo->data->forks[right_fork]) != 0)
			return (printf("pthread_mutex_lock failed\n"), 1);
		print_status(philo, "has taken a fork");
	}
	else
	{
		if (help_take_fork(philo, left_fork, right_fork) == 1)
			return (1);
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
		if (pthread_mutex_unlock(&philo->data->forks[left_fork]) != 0)
			return (printf("pthread_mutex_unlock failed\n"), 1);
		if (pthread_mutex_unlock(&philo->data->forks[right_fork]) != 0)
			return (printf("pthread_mutex_unlock failed\n"));
	}
	else
	{
		usleep(100);
		if (pthread_mutex_unlock(&philo->data->forks[right_fork]) != 0)
			return (printf("pthread_mutex_unlock failed\n"), 1);
		if (pthread_mutex_unlock(&philo->data->forks[left_fork]) != 0)
			return (printf("pthread_mutex_unlock failed\n"), 1);
	}
	return (0);
}
