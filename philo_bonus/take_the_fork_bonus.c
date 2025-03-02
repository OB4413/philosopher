/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/03/02 16:00:32 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
		sem_wait(philo->philos[i].meal_lock);
		if (get_time() - philo->philos[i].last_meal_time > philo->time_to_die)
		{
			print_status(&philo->philos[i], "is died");
			sem_post(philo->philos[i].meal_lock);
			sem_wait(philo->death_lock);
			philo->someone_died = 1;
			sem_post(philo->death_lock);
			return (1);
		}
		sem_post(philo->philos[i].meal_lock);
		i++;
	}
	return (0);
}

int	help_take_fork(t_philosopher *philo)
{
	if (check_death(philo))
		return (1);
	usleep(2000);
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a fork");
	return (0);
}

int	take_the_fork(t_philosopher *philo)
{
	if ((philo->id + 1) % 2 == 0)
	{
		if (check_death(philo))
			return (1);
		sem_wait(philo->data->forks);
		print_status(philo, "has taken a fork");
		sem_wait(philo->data->forks);
		print_status(philo, "has taken a fork");
	}
	else
	{
		if (help_take_fork(philo) == 1)
			return (1);
	}
	return (0);
}

int	put_the_fork(t_philosopher *philo)
{
	if ((philo->id + 1) % 2 == 0)
	{
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
	}
	else
	{
		usleep(100);
		sem_post(philo->data->forks);
		sem_post(philo->data->forks);
	}
	return (0);
}
