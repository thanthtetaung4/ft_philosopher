/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: taung <taung@student.42singapore.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 21:44:02 by taung             #+#    #+#             */
/*   Updated: 2025/03/06 22:02:25 by taung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

/**
 * Gets the current time in milliseconds.
 * Returns the current time in milliseconds since the Epoch.
 */
long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/**
 * Validates the input arguments and parses them into the parsing_data structure.
 * Returns true if the input is valid, otherwise false.
 */
bool	input_vali(int argc, char **argv, t_parsing *parsing_data)
{
	parsing_data->meals_to_eat = -1;
	if (argc < 5 || argc > 6 || !ft_atoi_vali(argv[1],
			&parsing_data->num_philos) || !ft_atoi_vali(argv[2],
			&parsing_data->time_to_die) || !ft_atoi_vali(argv[3],
			&parsing_data->time_to_eat) || !ft_atoi_vali(argv[4],
			&parsing_data->time_to_sleep) || (argc == 6
			&& !ft_atoi_vali(argv[5], &parsing_data->meals_to_eat)))
	{
		printf("Theses are the required arguments : \n"
			"\tphilosopher count (integer)\n"
			"\ttime to die (milliseconds)\n\ttime to eat (milliseconds)\n"
			"\ttime to sleep (milliseconds)\n"
			"\tnumber of times each philosopher must eat (optional)\n"
			"\tall input must be positive\nInvalid input!\n");
		return (false);
	}
	return (true);
}

/**
 * Gets the relative time in milliseconds since the start of the simulation.
 * Returns the relative time in milliseconds.
 */
long long	get_relative_time(long long start_time)
{
	return (get_time() - start_time);
}

/**
 * Prints the current state of a philosopher.
 * The state is printed only if the simulation is still running.
 */
void	print_state(t_philo *philo, char *state)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->simulation_mutex);
	if (philo->data->simulation_running)
	{
		printf("%lld %d %s\n", get_relative_time(philo->data->start_time),
			philo->id, state);
	}
	pthread_mutex_unlock(&philo->data->simulation_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
