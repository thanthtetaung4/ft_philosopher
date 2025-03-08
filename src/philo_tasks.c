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

/**
 * Checks if the simulation is still running.
 * Returns true if the simulation is running, otherwise false.
 */
bool	is_simulation_running(t_philo *philo)
{
	bool	running;

	pthread_mutex_lock(&philo->data->simulation_mutex);
	running = philo->data->simulation_running;
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	return (running);
}

/**
 * Takes forks based on the philosopher's ID toprevent deadlock.
 * The last philosopher takes the right fork first, others take
 * the left fork first.
 */
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

/**
 * Handles the eating phase and tracks the meal status.
 * Updates the last meal time and increments the meals eaten count.
 * Releases the forks after eating.
 */
// void	eat_meal(t_philo *philo)
// {
// 	print_state(philo, "is eating");
// 	pthread_mutex_lock(&philo->data->simulation_mutex);
// 	philo->last_meal_time = get_time();
// 	philo->meals_eaten++;
// 	pthread_mutex_unlock(&philo->data->simulation_mutex);
// 	usleep(philo->time_to_eat * 1000);
// 	pthread_mutex_unlock(&philo->left_fork->mutex);
// 	pthread_mutex_unlock(&philo->right_fork->mutex);
// }
void	eat_meal(t_philo *philo)
{
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->data->simulation_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
}

/**
 * Handles the sleeping phase.
 * The philosopher sleeps for the specified time.
 */
void	sleep_phase(t_philo *philo)
{
	print_state(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
}

/**
 * Handles the thinking phase with calculated thinking time.
 * The philosopher thinks for a calculated time based on the
 * time to die, eat, and sleep.
 */
void	think_phase(t_philo *philo)
{
	int	time_to_think;

	print_state(philo, "is thinking");
	pthread_mutex_lock(&philo->data->simulation_mutex);
	time_to_think = (philo->time_to_die
			- get_relative_time
			(philo->last_meal_time)
			- philo->time_to_eat
			- philo->time_to_sleep) / 2;
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	if (time_to_think > 0)
	{
		usleep(time_to_think * 1000);
	}
}
