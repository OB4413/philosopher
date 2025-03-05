/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin_and_monitor_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:49:07 by obarais           #+#    #+#             */
/*   Updated: 2025/03/04 17:52:14 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// ft_usleep function for sleep in microsecond
void	ft_usleep(t_philosopher *philo,long int duration)
{
	long int	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < duration)
	{
		sem_wait(philo->data->check_dead);
		if (get_time() - philo->last_meal_time > philo->data->time_to_die)
		{
			sem_post(philo->data->check_dead);
			print_status(philo, "is died");
			exit(1);
		}
		sem_post(philo->data->check_dead);
		usleep(500);
	}
}

// print_status function for print status of philosopher
void	print_status(t_philosopher *philo, char *status)
{
	sem_wait(philo->data->write_lock);
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id + 1,
	status);
	sem_post(philo->data->write_lock);
	sem_wait(philo->data->check_dead);
	if (get_time() - philo->last_meal_time > philo->data->time_to_die)
	{
		sem_post(philo->data->check_dead);
		print_status(philo, "is died");
		exit(1);
	}
	sem_post(philo->data->check_dead);
}

// function for philosopher routine
void	*philosopher_routine(void *phil)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)phil;
	while (1)
	{
		print_status(philo, "is thinking");
		if (philo->num_eat != philo->data->num_must_eat)
		{
			if (take_the_fork(philo) == 1)
				return (NULL);
			sem_wait(philo->meal_lock);
			philo->last_meal_time = get_time();
			sem_post(philo->meal_lock);
			print_status(philo, "is eating");
			philo->num_eat++;
			ft_usleep(philo, philo->data->time_to_eat);
		}
		print_status(philo, "is sleeping");
		ft_usleep(philo, philo->data->time_to_sleep);
		sem_wait(philo->data->check_dead);
		if (get_time() - philo->last_meal_time > philo->data->time_to_die)
		{
			sem_post(philo->data->check_dead);
			print_status(philo, "is died");
			exit(1);
		}
		sem_post(philo->data->check_dead);
		put_the_fork(philo);
		usleep(200);
	}
	return (NULL);
}
