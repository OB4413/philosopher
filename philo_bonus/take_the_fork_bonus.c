/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_the_fork_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 09:20:44 by obarais           #+#    #+#             */
/*   Updated: 2025/03/05 10:24:55 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	take_the_fork(t_philosopher *philo)
{
	if ((philo->id + 1) % 2 == 0)
		usleep(200);
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
