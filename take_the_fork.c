/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/02/26 13:41:50 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_the_fork(t_philosopher *philo)
{
	int	right_fork;
	int	left_fork;

	left_fork = (philo->id + philo->data->num_philos - 1)
		% philo->data->num_philos;
	right_fork = philo->id % philo->data->num_philos;
	if (philo->id % 2 == 0)
	{
		if (check_death(philo))
			return 1;
		if (pthread_mutex_lock(&philo->data->forks[left_fork]) != 0)
			return (printf("pthread_mutex_lock failed\n"), 1);
		print_status(philo, "has taken a fork");
		if (pthread_mutex_lock(&philo->data->forks[right_fork]) != 0)
			return (printf("pthread_mutex_lock failed\n"), 1);
		print_status(philo, "has taken a fork");
	}
	else
	{
		if (check_death(philo))
			return 1;
		usleep(2000);
		if (pthread_mutex_lock(&philo->data->forks[right_fork]) != 0)
			return (printf("pthread_mutex_lock failed\n"), 1);
		print_status(philo, "has taken a fork");
		if (pthread_mutex_lock(&philo->data->forks[left_fork]) != 0)
			return (printf("pthread_mutex_lock failed\n"), 1);
		print_status(philo, "has taken a fork");
	}
	return (0);
}

int	put_the_fork(t_philosopher *philo)
{
	int right_fork;
	int left_fork;

	left_fork = (philo->id + philo->data->num_philos - 1)
		% philo->data->num_philos;
	right_fork = philo->id % philo->data->num_philos;

	if (philo->id % 2 == 0)
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