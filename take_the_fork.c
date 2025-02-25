/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/02/25 15:54:12 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_the_fork(t_philosopher *philo)
{
	int	right_fork;
	int	left_fork;

	left_fork = (philo->id + philo->data->num_philos - 1)
		% philo->data->num_philos;
	right_fork = philo->id % philo->data->num_philos;
	if (philo->id % 2 == 0)
	{
		if (check_death(philo))
		{
			pthread_mutex_unlock(&philo->data->forks[left_fork]);
			return ;
		}
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		print_status(philo, "has taken a fork");
	}
	else
	{
		if (check_death(philo))
		{
			pthread_mutex_unlock(&philo->data->forks[right_fork]);
			return ;
		}
		usleep(2000);
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		print_status(philo, "has taken a fork");
	}
}

void	put_the_fork(t_philosopher *philo)
{
	int right_fork;
	int left_fork;

	left_fork = (philo->id + philo->data->num_philos - 1)
		% philo->data->num_philos;
	right_fork = philo->id % philo->data->num_philos;

	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
	}
	else
	{
		usleep(100);
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
	}
}