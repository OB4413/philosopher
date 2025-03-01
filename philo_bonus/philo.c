/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:16 by obarais           #+#    #+#             */
/*   Updated: 2025/02/28 08:38:07 by obarais          ###   ########.fr       */
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
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-' || str[0] == '\0')
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

	if (gettimeofday(&time, NULL) == -1)
		return (printf("gettimeofday failed\n"), -1);
	res = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (res);
}

// for < 2 philosophers and arguments
int	error(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("./philo num_philos time_die time_eat time_sleep [must_eat]\n");
		return (1);
	}
	if (ft_atoi(av[1]) < 2)
	{
		if (ft_atoi(av[1]) == 1)
		{
			printf("%d %s\n", 0, "1 has taken a fork");
			if (usleep(ft_atoi(av[2]) * 1000) == -1)
				return (printf("usleep failed\n"), 1);
			printf("%d %s\n", ft_atoi(av[2]), "1 died");
		}
		else
			return (printf("Error: Number of philosophers must be > 0\n"), 1);
		return (1);
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
		if (pthread_mutex_init(&data->philos[i].meal_lock, NULL) != 0)
			return (printf("pthread mutex init failed\n"), 1);
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
	if (error(ac, av) == 1)
		return (1);
	if (init_the_data(&data, av, ac) == 1)
		return (1);
	if (init_philo(&data) == 1)
		return (1);
	if (pthread_mutex_init(&data.write_lock, NULL) != 0
		|| pthread_mutex_init(&data.death_lock, NULL) != 0)
		return (printf("pthread mutex init failed\n"), 1);
	while (i < data.num_philos)
	{
		if (pthread_create(&data.philo[i], NULL, philosopher_routine,
				&data.philos[i]) != 0)
			return (printf("pthread_creat failed\n"), 1);
		i++;
	}
	if (pthread_create(&data.monitor, NULL, monitor_routine, &data) != 0)
		return (printf("pthread_creat failed\n"), 1);
	if (help_main(&data) == 1)
		return (1);
	return (0);
}
