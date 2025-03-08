/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:16 by obarais           #+#    #+#             */
/*   Updated: 2025/03/08 11:05:14 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// ft_atoi function for parameter to integer
int	ft_atoi(const char *str)
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
		nbr = (nbr * 10) + (str[i++] - '0');
		if (nbr > INT_MAX)
			return (-1);
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
	{
		printf("./philo num_philos time_die time_eat time_sleep [must_eat]\n");
		return (1);
	}
	if (ft_atoi(av[1]) < 2)
	{
		data->num_philos = ft_atoi(av[1]);
		data->time_to_die = ft_atoi(av[2]);
		data->time_to_eat = ft_atoi(av[3]);
		data->time_to_sleep = ft_atoi(av[4]);
		if (data->num_philos > 200 || data->time_to_die <= 0
			|| data->time_to_eat <= 0 || data->time_to_sleep <= 0)
			return (printf("Error: some parameter is bad\n"), 1);
		if (ft_atoi(av[1]) == 1)
		{
			printf("%d %s\n", 0, "is thinking");
			if (usleep(ft_atoi(av[2]) * 1000) == -1)
				return (printf("usleep failed\n"), 1);
			return (printf("%d %s\n", ft_atoi(av[2]), "1 died"), 1);
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
	while (i < data.num_philos)
	{
		data.philos[i].pid = fork();
		if (data.philos[i].pid < 0)
		{
			kill_processes(&data);
			return (clean_all(&data), printf("fork failed\n"), 1);
		}
		else if (data.philos[i].pid == 0)
			philosopher_routine(&data.philos[i]);
		i++;
	}
	if (help_main(&data) == 1)
		return (1);
	return (0);
}
