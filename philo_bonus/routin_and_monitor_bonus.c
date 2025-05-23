/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routin_and_monitor_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:49:07 by obarais           #+#    #+#             */
/*   Updated: 2025/03/08 13:35:39 by obarais          ###   ########.fr       */
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
		if (cheak_if_dead(philo) == 0)
			return ;
		usleep(50);
	}
}

// print_status function for print status of philosopher
void	print_status(t_philosopher *philo, char *status)
{
	sem_wait(philo->data->write_lock);
	if (cheak_if_dead(philo) == 0)
	{
		sem_post(philo->data->write_lock);
		return ;
	}
	printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id + 1,
		status);
	sem_post(philo->data->write_lock);
}

void	help_routine(t_philosopher *philo)
{
	while (1)
	{
		if (cheak_if_dead(philo) == 0)
			return ;
		print_status(philo, "is thinking");
		help_2routine(philo);
		put_the_fork(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo, philo->data->time_to_sleep);
		if (philo->num_eat == philo->data->num_must_eat)
		{
			philo->id = -1;
			return ;
		}
		if (cheak_if_dead(philo) == 0)
			return ;
	}
	return ;
}

void	*monitor_philo(void *phil)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)phil;
	while (1 && philo->id != -1)
	{
		sem_wait(philo->data->check_dead);
		if (philo->data->sameone_dead->__align == 0)
			return (sem_post(philo->data->check_dead), NULL);
		sem_post(philo->data->check_dead);
		sem_wait(philo->data->meal_lock);
		if ((get_time() - philo->last_meal_time) >= philo->data->time_to_die)
		{
			print_status(phil, "is died");
			sem_wait(philo->data->check_dead);
			philo->data->sameone_dead->__align = 0;
			sem_post(philo->data->check_dead);
			sem_post(philo->data->meal_lock);
			return (NULL);
		}
		sem_post(philo->data->meal_lock);
		usleep(100);
	}
	return (NULL);
}

// function for philosopher routine
void	philosopher_routine(void *phil)
{
	pthread_t		monitor;
	t_philosopher	*philo;

	philo = (t_philosopher *)phil;
	pthread_create(&monitor, NULL, monitor_philo, phil);
	help_routine(philo);
	pthread_join(monitor, NULL);
	clean_all(philo->data);
	exit(0);
}
