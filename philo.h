/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:52 by obarais           #+#    #+#             */
/*   Updated: 2025/02/25 11:42:57 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
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
	long int		last_meal_time;
	pthread_mutex_t	meal_lock;
	struct s_data	*data;
	pthread_t		thread;
}					t_philosopher;

typedef struct s_data
{
	int				num_philos;
	long int		start_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_must_eat;
	int				someone_died;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	t_philosopher	*philos;
}					t_data;

void				print_status(t_philosopher *philo, char *status);
void				*philosopher_routine(void *phil);
void				*monitor_routine(void *data);
long int			get_time(void);
void				ft_usleep(t_philosopher *philo, long int duration);
int					init_the_data(t_data *data, char **av, int ac);
int					ft_atoi(const char *str);
void				take_the_fork(t_philosopher *philo);
void				put_the_fork(t_philosopher *philo);
int					check_death(t_philosopher *phil);

#endif