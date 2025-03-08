/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:42:12 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 22:01:07 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

/**
 * Philosopher routine.
 * Each philosopher goes through the cycle of taking forks, eating,
 * sleeping, and thinking.
 * If there is only one philosopher, they handle the single philosopher case.
 * The routine continues until the simulation is no longer running.
 */
void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->num_philos == 1)
	{
		handle_single_philosopher(philo);
		return (NULL);
	}
	while (1)
	{
		if (!is_simulation_running(philo))
		{
			break ;
		}
		take_forks(philo);
		eat_meal(philo);
		sleep_phase(philo);
		think_phase(philo);
	}
	return (NULL);
}

/**
 * Monitor thread routine.
 * Continuously checks if any philosopher has died or
 if all meals are completed.
 * Stops the simulation if a philosopher dies or all meals are completed.
 * The routine continues until the simulation is no longer running.
 */
void	*monitor(void *arg)
{
	t_philo	*philos;
	int		i;
	bool	simulation_running;

	philos = (t_philo *)arg;
	simulation_running = true;
	while (simulation_running)
	{
		i = 0;
		while (i < philos[0].num_philos)
		{
			if (check_philosopher_death(&philos[i]))
				return (NULL);
			i++;
		}
		if (check_meals_completion(philos))
		{
			stop_simulation(&philos[0]);
			return (NULL);
		}
		simulation_running = get_simulation_status(&philos[0]);
		usleep(5000);
	}
	return (NULL);
}
