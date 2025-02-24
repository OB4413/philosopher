/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:16 by obarais           #+#    #+#             */
/*   Updated: 2025/02/23 18:33:19 by obarais          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// ft_atoi function for parameter to integer
long int	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long int	nbr;

	i = 0;
	sign = 1;
	nbr = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
		nbr = (nbr * 10) + (str[i++] - '0');
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] && !(str[i] >= '0' && str[i] <= '9'))
		return (0);
	return (nbr * sign);
}

// ft_usleep function for sleep in microsecond
void	ft_usleep(long int time)
{
	int i;

	i = 0;
	while (i++ < 1000)
		usleep(time);
}

// get_time function for get time in milliseconds
long long	get_time(void)
{
	struct timeval	time;
	long long		res;

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
			ft_usleep(ft_atoi(av[2]));
			printf("%ld %s\n", ft_atoi(av[2]), "1 died");
		}
		else
			return (printf("Error: Number of philosophers must be > 0\n"));
		return (1);
	}
	return (0);
}

// print_status function for print status of philosopher
void	print_status(t_philosopher *philo, char *status)
{
	pthread_mutex_lock(&philo->data->write_lock);
	if (!philo->data->someone_died)
		printf("%lld %d %s\n", get_time() - philo->data->start_time, philo->id, status);
	pthread_mutex_unlock(&philo->data->write_lock);
}

// function for philosopher routine
void	*philosopher_routine(void *phil)
{
	t_philosopher	*philo = (t_philosopher *)phil;
	
	while (!philo->data->someone_died)
	{
		print_status(philo, "is thinking");
		
		pthread_mutex_lock(&philo->data->forks[philo->id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[(philo->id + 1) % philo->data->num_philos]);
		print_status(philo, "has taken a fork");

		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->meal_lock);
		print_status(philo, "is eating");
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1) % philo->data->num_philos]);

		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
	}
	return (NULL);
}

// function for monitor routine
void	*monitor_routine(void *data)
{
	t_data	*philo = (t_data *)data;
	int		i;

	while (!philo->someone_died)
	{
		i = 1;
		while (i <= philo->num_philos)
		{
			pthread_mutex_lock(&philo->philos[i].meal_lock);
			if (get_time() - philo->philos[i].last_meal_time > philo->time_to_die)
			{
				print_status(&philo->philos[i], "died");
				philo->someone_died = 1;
				break ;
			}
			pthread_mutex_unlock(&philo->philos[i].meal_lock);
			i++;
		}
	}
	return (NULL);
}

// the form of av program [number of philo] [time die] [time eat] [time sleep] [number of times each philosopher must eat]
int	main(int ac, char **av)
{
	int i = 1;
	t_data data;

	if (error(ac, av) == 1)
		return (1);
	data.num_philos = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);
	data.num_must_eat = ft_atoi(av[5]);
	data.someone_died = 0;
	data.start_time = get_time();
	
	pthread_t philo[data.num_philos];
	pthread_t monitor;

	while (i <= data.num_philos)
	{
		data.philos[i].id = i;
		data.philos[i].last_meal_time = data.start_time;
		pthread_mutex_init(&data.philos[i].meal_lock, NULL);
		data.philos[i].data = &data;
		i++;
	}
	pthread_mutex_init(&data.write_lock, NULL);
	i = 1;
	while (i <= data.num_philos)
	{
		pthread_create(&philo[i], NULL, philosopher_routine, &data.philos[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_routine, &data);
	
	i = 1;
	while (i <= data.num_philos)
	{
		pthread_join(philo[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	i = 1;
	while (i <= data.num_philos)
	{
		pthread_mutex_destroy(&data.philos[i].meal_lock);
		i++;
	}
	pthread_mutex_destroy(&data.write_lock);
	return (0);
}
