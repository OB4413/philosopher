/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all_helpmain_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:43:59 by obarais           #+#    #+#             */
/*   Updated: 2025/03/05 10:46:45 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_all(t_data *data)
{
	int i;

	sem_close(data->write_lock);
	sem_close(data->forks);
	sem_close(data->check_dead);
	i = 0;
	while (i < data->num_philos)
	{
		sem_close(data->philos[i].meal_lock);
		sem_unlink("/meal_lock");
		i++;
	}
	sem_unlink("/check_dead");
	sem_unlink("/forks");
	sem_unlink("/write_lock");
	free(data->philos);
}

int	help_main(t_data *data)
{
	int	i;

	i = 0;
	pid_t	pid;

	pid = waitpid(-1, &i, 0);
	while (pid > 0)
	{
		if (WIFEXITED(i) || WIFSIGNALED(i))
		{
			pid = waitpid(-1, &i, 0);
			continue;
		}
	}
	while (wait(NULL) > 0)
		;
	kill_processes(data);
	clean_all(data);
	return (0);
}

// function for allocated memory
void	*ft_allocated(t_data *data)
{
	data->philos = (t_philosopher *)malloc(sizeof(t_philosopher) * data->num_philos);
	if (data->philos == NULL)
		return (printf("malloc failed\n"), NULL);
	data->forks = sem_open("/forks", O_CREAT, 0644, data->num_philos);
	data->write_lock = sem_open("/write_lock", O_CREAT, 0644, 1);
	data->check_dead = sem_open("/check_dead", O_CREAT, 0644, 1);
	if (data->forks == SEM_FAILED || data->write_lock == SEM_FAILED)
		return (printf("sem_open failed\n"), NULL);
	return (data);
}

// initialize data
int	init_the_data(t_data *data, char **av, int ac)
{
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->num_must_eat = INT_MAX;
	data->start_time = get_time();
	if (data->num_philos <= 0 || data->num_philos > 200
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (printf("Error: some parameter is bad\n"), 1);
	if (data->start_time == -1)
		return (1);
	if (ac == 6)
	{
		data->num_must_eat = ft_atoi(av[5]);
		if (data->num_must_eat < 0)
			return (printf("Error: some parameter is bad\n"), 1);
	}
	if (ft_allocated(data) == NULL)
		return (1);
	return (0);
}
