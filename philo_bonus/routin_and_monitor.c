/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin_and_monitor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:49:07 by obarais           #+#    #+#             */
/*   Updated: 2025/03/02 12:00:00 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// check if the philosopher is dead
int	check_death(t_philosopher *phil)
{
	int	result;

	sem_wait(phil->data->death_lock);
	result = phil->data->someone_died;
	sem_post(phil->data->death_lock);
	return (result);
}

// ft_usleep function for sleep in microsecond
void	ft_usleep(t_philosopher *philo, long int duration)
{
	long int	start_time;

	start_time = get_time();
	while ((get_time() - start_time) < duration)
	{
		if (check_death(philo))
			break ;
		usleep(500);
	}
}

// print_status function for print status of philosopher
void	print_status(t_philosopher *philo, char *status)
{
	if (check_death(philo) == 1)
		return ;
	sem_wait(philo->data->write_lock);
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id + 1,
		status);
	sem_post(philo->data->write_lock);
}

// function for philosopher routine
void	*philosopher_routine(void *phil)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)phil;
	while (1)
	{
		if (check_death(philo))
			return (NULL);
		print_status(philo, "is thinking");
		if (take_the_fork(philo) == 1)
			return (NULL);
		if (philo->num_eat != philo->data->num_must_eat)
		{
			sem_wait(philo->meal_lock);
			philo->last_meal_time = get_time();
			sem_post(philo->meal_lock);
			print_status(philo, "is eating");
			philo->num_eat++;
			ft_usleep(philo, philo->data->time_to_eat);
		}
		if (help_routine(philo) == 1)
			return (NULL);
		print_status(philo, "is sleeping");
		ft_usleep(philo, philo->data->time_to_sleep);
	}
	return (NULL);
}

// function for monitor routine
void	*monitor_routine(void *data)
{
	t_data	*philo;

	philo = (t_data *)data;
	while (1)
	{
		sem_wait(philo->death_lock);
		if (philo->someone_died)
		{
			sem_post(philo->death_lock);
			return (NULL);
		}
		sem_post(philo->death_lock);
		if (help_monitor(philo) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
