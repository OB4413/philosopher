/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin_and_monitor_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:49:07 by obarais           #+#    #+#             */
/*   Updated: 2025/03/06 14:01:23 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// ft_usleep function for sleep in microsecond
void	ft_usleep(t_philosopher *philo, long int duration)
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
			kill_processes(philo->data);
			clean_all(philo->data);
			exit(0);
		}
		sem_post(philo->data->check_dead);
		usleep(50);
	}
}

// print_status function for print status of philosopher
void	print_status(t_philosopher *philo, char *status)
{
	sem_wait(philo->data->write_lock);
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id + 1,
		status);
	sem_post(philo->data->write_lock);
}

void	*help_routine(t_philosopher *philo)
{
	while (1)
	{
		if (get_time() - philo->last_meal_time >= philo->data->time_to_die)
			break ;
		print_status(philo, "is thinking");
		if (help_2routine(philo) == NULL)
			return (NULL);
		print_status(philo, "is sleeping");
		sem_wait(philo->data->check_dead);
		if (get_time() - philo->last_meal_time >= philo->data->time_to_die)
			break ;
		sem_post(philo->data->check_dead);
		ft_usleep(philo, philo->data->time_to_sleep);
		if (philo->num_eat != philo->data->num_must_eat)
			put_the_fork(philo);
		usleep(100);
	}
	return (philo);
}

// function for philosopher routine
void	*philosopher_routine(void *phil)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)phil;
	if (help_routine(philo) == NULL)
	{
		kill_processes(philo->data);
		clean_all(philo->data);
		exit(0);
	}
	sem_post(philo->data->check_dead);
	print_status(philo, "is died");
	kill_processes(philo->data);
	clean_all(philo->data);
	exit(0);
	return (NULL);
}
