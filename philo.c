/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:16 by obarais           #+#    #+#             */
/*   Updated: 2025/02/25 15:10:57 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ft_atoi function for parameter to integer
int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		return (-1);
	if (str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		if ((nbr * sign) > INT_MAX)
			return (-1);
		nbr = (nbr * 10) + (str[i++] - '0');
	}
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] && !(str[i] >= '0' && str[i] <= '9'))
		return (-1);
	return (nbr * sign);
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
int	error(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	if (ft_atoi(av[1]) < 2)
	{
		if (ft_atoi(av[1]) == 1)
		{
			printf("%d %s\n", 0, "1 has taken a fork");
			usleep(ft_atoi(av[2]) * 1000);
			printf("%d %s\n", ft_atoi(av[2]), "1 died");
		}
		else
			return (printf("Error: Number of philosophers must be > 0\n"));
		return (1);
	}
	return (0);
}

// the form of av program [number of philo] [time die] [time eat] [time sleep] [number of times each philosopher must eat]
int	main(int ac, char **av)
{
	int			i;
	t_data		data;

	i = 0;

	// check if the number of arguments is correct
	if (error(ac, av) == 1)
		return (1);
	
	// initialize data
	if (init_the_data(&data, av, ac) == 1)
		return (1);
	
	// initialize data
	while (i < data.num_philos)
	{
		data.philos[i].id = i;
		data.philos[i].last_meal_time = data.start_time;
		pthread_mutex_init(&data.philos[i].meal_lock, NULL);
		data.philos[i].data = &data;
		i++;
	}
	pthread_mutex_init(&data.write_lock, NULL);
	pthread_mutex_init(&data.death_lock, NULL);
	
	// create the saver of ID threads
	pthread_t	philo[data.num_philos];
	pthread_t	monitor;

	// create threads
	i = 0;
	while (i < data.num_philos)
	{
		pthread_create(&philo[i], NULL, philosopher_routine, &data.philos[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, &data);

	// join threads
	i = 0;
	while (i < data.num_philos)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);

	// free memory
	i = 0;
	while (i < data.num_philos)
	{
		pthread_mutex_destroy(&data.philos[i].meal_lock);
		pthread_mutex_destroy(&data.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data.write_lock);
	pthread_mutex_destroy(&data.death_lock);
	free(data.forks);
	free(data.philos);
	
	return (0);
}
