/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:16 by obarais           #+#    #+#             */
/*   Updated: 2025/03/07 14:39:00 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ft_atoi function for parameter to integer
long	ft_atoi(const char *str)
{
	int		i;
	long	nbr;

	i = 0;
	nbr = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-' || str[0] == '\0')
		return (-1);
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		if (nbr > INT_MAX)
			return (-1);
		nbr = (nbr * 10) + (str[i++] - '0');
	}
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] && !(str[i] >= '0' && str[i] <= '9'))
		return (-1);
	return (nbr);
}

// get_time function for get time in milliseconds
long int	get_time(void)
{
	struct timeval	time;
	long int		res;

	gettimeofday(&time, NULL);
	res = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (res);
}

// for < 2 philosophers and arguments
int	error(int ac, char **av, t_data *data)
{
	if (ac < 5 || ac > 6)
		return (printf("./philo num_philos t_die t_eat t_sleep [must_eat]\n"),
			1);
	if (ft_atoi(av[1]) < 2)
	{
		data->num_philos = ft_atoi(av[1]);
		data->time_to_die = ft_atoi(av[2]);
		data->time_to_eat = ft_atoi(av[3]);
		data->time_to_sleep = ft_atoi(av[4]);
		if (data->num_philos > 200 || data->num_philos <= 0
			|| data->time_to_die <= 0 || data->time_to_eat <= 0
			|| data->time_to_sleep <= 0)
			return (printf("Error: some parameter is bad\n"), 1);
		if (ft_atoi(av[1]) == 1)
		{
			printf("%d %s\n", 0, "is thinking");
			if (usleep(ft_atoi(av[2]) * 1000) == -1)
				return (printf("usleep failed\n"), 1);
			return (printf("%ld %s\n", ft_atoi(av[2]), "1 died"), 1);
		}
		else
			return (printf("Error: Number of philosophers must be > 0\n"), 1);
	}
	return (0);
}

int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i;
		data->philos[i].num_eat = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		i++;
	}
	return (0);
}

// av ./program [number of philo] [time die] [time eat] [time sleep] [must eat]
int	main(int ac, char **av)
{
	int		i;
	t_data	data;

	i = 0;
	if (error(ac, av, &data) == 1)
		return (1);
	if (init_the_data(&data, av, ac) == 1)
		return (1);
	init_philo(&data);
	pthread_mutex_init(&data.write_lock, NULL);
	pthread_mutex_init(&data.death_lock, NULL);
	while (i < data.num_philos)
	{
		pthread_create(&data.philo[i], NULL, philosopher_routine,
			&data.philos[i]);
		i++;
	}
	pthread_create(&data.monitor, NULL, monitor_routine, &data);
	help_main(&data);
	return (0);
}
