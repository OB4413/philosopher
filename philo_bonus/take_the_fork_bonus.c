/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/03/04 17:30:50 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	take_the_fork(t_philosopher *philo)
{
	if ((philo->id + 1) % 2 == 0)
	{
		sem_wait(philo->data->forks);
		print_status(philo, "has taken a forks");
	}
	else
	{
		usleep(100);
		sem_wait(philo->data->forks);
		print_status(philo, "has taken a forks");
	}
	sem_wait(philo->data->check_dead);
	if (get_time() - philo->last_meal_time > philo->data->time_to_die)
	{
		sem_post(philo->data->check_dead);
		print_status(philo, "is died");
		exit(1);
	}
	sem_post(philo->data->check_dead);
	return (0);
}

int	put_the_fork(t_philosopher *philo)
{
	if ((philo->id + 1) % 2 == 0)
		sem_post(philo->data->forks);
	else
	{
		usleep(100);
		sem_post(philo->data->forks);
	}
	sem_wait(philo->data->check_dead);
	if (get_time() - philo->last_meal_time > philo->data->time_to_die)
	{
		sem_post(philo->data->check_dead);
		print_status(philo, "is died");
		exit(1);
	}
	sem_post(philo->data->check_dead);
	return (0);
}
