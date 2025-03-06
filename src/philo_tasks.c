/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:40:11 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 22:00:57 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

// Check if simulation is still running
bool	is_simulation_running(t_philo *philo)
{
	bool	running;

	pthread_mutex_lock(&philo->data->simulation_mutex);
	running = philo->data->simulation_running;
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	return (running);
}

// Take forks based on philosopher's ID to prevent deadlock
void	take_forks(t_philo *philo)
{
	if (philo->id == philo->num_philos - 1)
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork->mutex);
		print_state(philo, "has taken a fork");
	}
}

// Eat and track meal status
void	eat_meal(t_philo *philo)
{
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->data->simulation_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

// Sleep phase
void	sleep_phase(t_philo *philo)
{
	print_state(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
}

// Think phase with calculated thinking time
void	think_phase(t_philo *philo)
{
	int	time_to_think;

	print_state(philo, "is thinking");
	time_to_think = ((philo->time_to_die - (philo->time_to_eat
					+ philo->time_to_sleep)) / 2);
	if (time_to_think > 0)
	{
		usleep(time_to_think * 1000);
	}
}
