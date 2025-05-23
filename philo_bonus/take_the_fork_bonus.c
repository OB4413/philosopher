/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/03/08 11:32:07 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	take_the_fork(t_philosopher *philo)
{
	if ((philo->id + 1) % 2 != 0)
		usleep(1000);
	sem_wait(philo->data->forks);
	sem_wait(philo->data->forks);
	print_status(philo, "has taken a forks");
	return (0);
}

int	put_the_fork(t_philosopher *philo)
{
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	return (0);
}

void	kill_processes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].pid > 0)
			kill(data->philos[i].pid, SIGKILL);
		i++;
	}
}

void	help_2routine(t_philosopher *philo)
{
	take_the_fork(philo);
	if (philo->num_eat != philo->data->num_must_eat)
	{
		sem_wait(philo->data->meal_lock);
		philo->last_meal_time = get_time();
		sem_post(philo->data->meal_lock);
		print_status(philo, "is eating");
		philo->num_eat++;
		ft_usleep(philo, philo->data->time_to_eat);
	}
	return ;
}

int	cheak_if_dead(t_philosopher *philo)
{
	int	re;

	sem_wait(philo->data->check_dead);
	re = philo->data->sameone_dead->__align;
	sem_post(philo->data->check_dead);
	return (re);
}
