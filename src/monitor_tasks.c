/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_tasks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:43:28 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 21:56:42 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

// Handle single philosopher case
void	handle_single_philosopher(t_philo *philo)
{
	print_state(philo, "has taken a fork");
	usleep(philo->time_to_die * 1000);
}

// Check if a philosopher has died
bool	check_philosopher_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->simulation_mutex);
	if (get_time() - philo->last_meal_time > philo->time_to_die)
	{
		philo->data->simulation_running = 0;
		pthread_mutex_unlock(&philo->data->simulation_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d died\n", get_relative_time(philo->data->start_time),
			philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	return (false);
}

// Check if all philosophers have completed their meals
bool	check_meals_completion(t_philo *philos)
{
	int		i;
	bool	all_completed;

	i = 0;
	all_completed = true;
	if (philos[0].meals_to_eat == -1)
		return (false);
	while (i < philos[0].num_philos)
	{
		pthread_mutex_lock(&philos[i].data->simulation_mutex);
		if (philos[i].meals_to_eat != -1
			&& philos[i].meals_eaten < philos[i].meals_to_eat)
		{
			all_completed = false;
		}
		pthread_mutex_unlock(&philos[i].data->simulation_mutex);
		i++;
	}
	return (all_completed);
}

// Set simulation status to stop
void	stop_simulation(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->simulation_mutex);
	philo->data->simulation_running = 0;
	pthread_mutex_unlock(&philo->data->simulation_mutex);
}

// Get current simulation status
bool	get_simulation_status(t_philo *philo)
{
	bool	status;

	pthread_mutex_lock(&philo->data->simulation_mutex);
	status = philo->data->simulation_running;
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	return (status);
}
