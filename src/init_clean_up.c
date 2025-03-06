/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_clean_up.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:47:41 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 22:01:19 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void	init_data(t_data *data, t_parsing parsing_data)
{
	int	i;

	data->num_philos = parsing_data.num_philos;
	data->simulation_running = 1;
	data->meals_completed = 0;
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->simulation_mutex, NULL);
	data->start_time = get_time();
	data->forks = malloc(sizeof(t_fork) * parsing_data.num_philos);
	i = 0;
	while (i < parsing_data.num_philos)
	{
		pthread_mutex_init(&data->forks[i].mutex, NULL);
		data->forks[i].id = i;
		i++;
	}
}

void	init_philo(t_data *data, t_parsing parsing_data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * parsing_data.num_philos);
	data->philo_threads = malloc(sizeof(pthread_t) * parsing_data.num_philos);
	i = 0;
	while (i < parsing_data.num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].num_philos = parsing_data.num_philos;
		data->philos[i].time_to_die = parsing_data.time_to_die;
		data->philos[i].time_to_eat = parsing_data.time_to_eat;
		data->philos[i].time_to_sleep = parsing_data.time_to_sleep;
		data->philos[i].meals_to_eat = parsing_data.meals_to_eat;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_time();
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1)
			% parsing_data.num_philos];
		data->philos[i].data = data;
		i++;
	}
}

void	clean_up(t_data *data, t_parsing *parsing_data)
{
	int	i;

	i = 0;
	while (i < parsing_data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->simulation_mutex);
	free(data->forks);
	free(data->philos);
	free(data->philo_threads);
}
