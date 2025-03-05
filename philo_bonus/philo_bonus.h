/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:52 by obarais           #+#    #+#             */
/*   Updated: 2025/03/05 09:51:00 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <limits.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <sys/wait.h>

typedef struct s_philosopher
{
	int				id;
	pid_t			pid;
	int				num_eat;
	long int		last_meal_time;
	sem_t			*meal_lock;
	struct s_data	*data;
}					t_philosopher;

typedef struct s_data
{
	int				num_philos;
	long int		start_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_must_eat;
	sem_t			*forks;
	sem_t			*write_lock;
	sem_t			*check_dead;
	t_philosopher	*philos;
}					t_data;

void				print_status(t_philosopher *philo, char *status);
void				*philosopher_routine(void *phil);
void				*monitor_routine(void *data);
long int			get_time(void);
void				ft_usleep(t_philosopher *philo, long int duration);
int					init_the_data(t_data *data, char **av, int ac);
int					ft_atoi(const char *str);
int					take_the_fork(t_philosopher *philo);
int					put_the_fork(t_philosopher *philo);
int					help_main(t_data *data);
int					help_routine(t_philosopher *philo);
void				clean_all(t_data *data);
void				kill_processes(t_data *data);

#endif