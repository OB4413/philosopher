/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin_and_monitor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:49:07 by obarais           #+#    #+#             */
/*   Updated: 2025/03/07 17:09:38 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// check if the philosopher is dead
int	check_death(t_philosopher *phil)
{
	int	result;

	pthread_mutex_lock(&phil->data->death_lock);
	result = phil->data->someone_died;
	pthread_mutex_unlock(&phil->data->death_lock);
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
		usleep(50);
	}
}

// print_status function for print status of philosopher
void	print_status(t_philosopher *philo, char *status)
{
	pthread_mutex_lock(&philo->data->write_lock);
	if (check_death(philo) == 1)
	{
		pthread_mutex_unlock(&philo->data->write_lock);
		return ;
	}
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id + 1,
		status);
	pthread_mutex_unlock(&philo->data->write_lock);
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
		take_the_fork(philo);
		if (philo->num_eat != philo->data->num_must_eat)
		{
			pthread_mutex_lock(&philo->data->meal_lock);
			philo->last_meal_time = get_time();
			pthread_mutex_unlock(&philo->data->meal_lock);
			print_status(philo, "is eating");
			philo->num_eat++;
			ft_usleep(philo, philo->data->time_to_eat);
		}
		if (help_routine(philo) == 1)
			return (NULL);
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
		pthread_mutex_lock(&philo->death_lock);
		if (philo->someone_died)
		{
			pthread_mutex_unlock(&philo->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->death_lock);
		if (help_monitor(philo) == 1)
			return (NULL);
		usleep(10);
	}
	return (NULL);
}
