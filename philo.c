/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obarais <obarais@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 11:17:16 by obarais           #+#    #+#             */
/*   Updated: 2025/02/24 11:03:28 by obarais          ###   ########.fr       */
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
	if (str[i] == '+' || str[i] == '-')
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

// ft_usleep function for sleep in microsecond
void	ft_usleep(long int time)
{
	int	i;

	i = 0;
	while (i++ < 1000)
		usleep(time);
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
			ft_usleep(ft_atoi(av[2]));
			printf("%d %s\n", ft_atoi(av[2]), "1 died");
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
		printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id,
			status);
	pthread_mutex_unlock(&philo->data->write_lock);
}

// function for philosopher routine
void	*philosopher_routine(void *phil)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)phil;
	while (!philo->data->someone_died)
	{
		print_status(philo, "is thinking");
		pthread_mutex_lock(&philo->data->forks[philo->id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[(philo->id + 1)
			% philo->data->num_philos]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meal_time = get_time();
		pthread_mutex_unlock(&philo->meal_lock);
		print_status(philo, "is eating");
		ft_usleep(philo->data->time_to_eat);
		pthread_mutex_unlock(&philo->data->forks[philo->id]);
		pthread_mutex_unlock(&philo->data->forks[(philo->id + 1)
			% philo->data->num_philos]);
		print_status(philo, "is sleeping");
		ft_usleep(philo->data->time_to_sleep);
	}
	return (NULL);
}

// function for monitor routine
void	*monitor_routine(void *data)
{
	t_data	*philo;
	int		i;

	philo = (t_data *)data;
	while (!philo->someone_died)
	{
		i = 0;
		while (i <= philo->num_philos)
		{
			pthread_mutex_lock(&philo->philos[i].meal_lock);
			if (get_time()
				- philo->philos[i].last_meal_time > philo->time_to_die)
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

// function for allocated memory
void	*ft_allocated(t_data *data)
{
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* data->num_philos);
	if (!data->forks)
		return NULL;
	data->philos = (t_philosopher *)malloc(sizeof(t_philosopher)
			* data->num_philos);
	if (!data->philos)
	{
		free(data->forks);
		return NULL;
	}
	return (data);
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
	
	// check if the number of times each philosopher must eat is given
	if (ac == 6)
	{
		data.num_must_eat = ft_atoi(av[5]);
		if (data.num_must_eat < 0)
			return (printf("Error: some parameter is bad\n"), 1);
	}
	
	// initialize data
	data.num_philos = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);
	data.someone_died = 0;
	data.start_time = get_time();
	if (data.num_philos < 0 || data.time_to_die < 0 || data.time_to_eat < 0
		|| data.time_to_sleep < 0)
		return (printf("Error: some parameter is bad\n"), 1);
	
	// allocate memory for data.philos and data.forks
	if (ft_allocated(&data) == NULL)
		return (printf("filed to allocate memory\n"), 1);
	
	// initialize mutex
	pthread_mutex_init(&data.write_lock, NULL);
	i = 0;
	while (i < data.num_philos)
	{
		pthread_mutex_init(&data.forks[i], NULL);
		i++;
	}

	// create the saver of ID threads
	pthread_t	philo[data.num_philos];
	pthread_t	monitor;

	// initialize data
	i = 0;
	while (i < data.num_philos)
	{
		data.philos[i].id = i;
		data.philos[i].last_meal_time = data.start_time;
		pthread_mutex_init(&data.philos[i].meal_lock, NULL);
		data.philos[i].data = &data;
		i++;
	}
	pthread_mutex_init(&data.write_lock, NULL);

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
		i++;
	}
	pthread_mutex_destroy(&data.write_lock);
	
	return (0);
}
