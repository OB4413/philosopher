/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:52 by obarais           #+#    #+#             */
/*   Updated: 2025/02/23 18:33:01 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_PHILOS 200

typedef struct s_philosopher
{
	int				id;
	long long		last_meal_time;
	pthread_mutex_t	meal_lock;
	struct s_data	*data;
}					t_philosopher;

typedef struct s_data
{
	int				num_philos;
	long long		start_time;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				num_must_eat;
	int				someone_died;
	pthread_mutex_t	forks[MAX_PHILOS];
	pthread_mutex_t	write_lock;
	t_philosopher	philos[MAX_PHILOS];
}					t_data;

#endif