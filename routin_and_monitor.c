/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin_and_monitor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:49:07 by obarais           #+#    #+#             */
/*   Updated: 2025/02/27 09:00:13 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// check if the philosopher is dead
int	check_death(t_philosopher *phil)
{
	int result;

	pthread_mutex_lock(&phil->data->death_lock);
	result = phil->data->someone_died;
	pthread_mutex_unlock(&phil->data->death_lock);
	return (result);
}


// ft_usleep function for sleep in microsecond
void	ft_usleep(t_philosopher *philo, long int duration)
{
	long int start_time = get_time();
	while ((get_time() - start_time) < duration)
	{
		if (check_death(philo))
			break;
		usleep(500);
	}
}

// print_status function for print status of philosopher
void	print_status(t_philosopher *philo, char *status)
{
	if (check_death(philo) == 1)
		return ;
	pthread_mutex_lock(&philo->data->write_lock);
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id + 1, status);
	pthread_mutex_unlock(&philo->data->write_lock);
}

// function for philosopher routine
void	*philosopher_routine(void *phil)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)phil;
	while (1)
	{
		if(check_death(philo))
			return (NULL);
		
		print_status(philo, "is thinking");
		
		if (take_the_fork(philo) == 1)
			return (NULL);
		
		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->meal_lock);
		
		print_status(philo, "is eating");
		ft_usleep(philo, philo->data->time_to_eat);
		
		if (put_the_fork(philo) == 1)
			return (NULL);

		if(check_death(philo))
			return (NULL);
		
		print_status(philo, "is sleeping");
		ft_usleep(philo , philo->data->time_to_sleep);
	}
	return (NULL);
}

// function for monitor routine
void	*monitor_routine(void *data)
{
	t_data	*philo;
	int		i;

	philo = (t_data *)data;
	while (1)
	{
		if (pthread_mutex_lock(&philo->death_lock) != 0)
			return (printf("pthread_mutex_lock failed\n"), NULL);
		if (philo->someone_died)
		{
			if (pthread_mutex_unlock(&philo->death_lock) != 0)
				return (printf("pthread_mutex_unlock failed\n"), NULL);
			return (NULL);
		}
		if (pthread_mutex_unlock(&philo->death_lock) != 0)
			return (printf("pthread_mutex_unlock failed\n"), NULL);

		i = 0;
		while (i < philo->num_philos)
		{
			if (pthread_mutex_lock(&philo->philos[i].meal_lock) != 0)
				return (printf("pthread_mutex_lock failed\n"), NULL);
			if (get_time() - philo->philos[i].last_meal_time > philo->time_to_die)
			{
				print_status(&philo->philos[i], "is died");
				if (pthread_mutex_unlock(&philo->philos[i].meal_lock) != 0)
					return (printf("pthread_mutex_unlock failed\n"), NULL);

				if (pthread_mutex_lock(&philo->death_lock) != 0)
					return (printf("pthread_mutex_lock failed\n"), NULL);
				philo->someone_died = 1;
				if (pthread_mutex_unlock(&philo->death_lock) != 0)
					return (printf("pthread_mutex_unlock failed\n"), NULL);
				
				return (NULL);
			}
			if (pthread_mutex_unlock(&philo->philos[i].meal_lock) != 0)
				return (printf("pthread_mutex_unlock failed\n"), NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
