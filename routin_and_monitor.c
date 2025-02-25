/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin_and_monitor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:49:07 by obarais           #+#    #+#             */
/*   Updated: 2025/02/25 15:00:34 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// check if the philosopher is dead
int	check_death(t_philosopher *phil)
{
	pthread_mutex_lock(&phil->data->death_lock);
	return (phil->data->someone_died);
	pthread_mutex_unlock(&phil->data->death_lock);
	return (0);
}


// ft_usleep function for sleep in microsecond
void	ft_usleep(t_philosopher *philo, long int duration)
{
	int i = 0;
	long int start_time = duration / 5;
	while ( i < 5 && !check_death(philo))
	{
		usleep(start_time * 1000);
		i++;
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
	while (!check_death(philo))
	{
		print_status(philo, "is thinking");
		
		if(check_death(philo))
			return (NULL);

		take_the_fork(philo);
		
		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->meal_lock);
		print_status(philo, "is eating");
		ft_usleep(philo, philo->data->time_to_eat);
		
		put_the_fork(philo);

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
	while (!check_death(philo->philos))
	{
		i = 0;
		while (i < philo->num_philos)
		{
			pthread_mutex_lock(&philo->philos[i].meal_lock);
			if (get_time()
				- philo->philos[i].last_meal_time > philo->time_to_die)
			{
				print_status(&philo->philos[i], "is died");
				pthread_mutex_lock(&philo->death_lock);
    			philo->someone_died = 1;
    			pthread_mutex_unlock(&philo->death_lock);
				pthread_mutex_unlock(&philo->philos[i].meal_lock);
				break ;
			}
			pthread_mutex_unlock(&philo->philos[i].meal_lock);
			i++;
		}
	}
	return (NULL);
}
